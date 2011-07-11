#include "driver.h"

#include <ntstrsafe.h>
#include <wdm.h>

#include "device.h"
#include "pnp.h"
#include "read.h"
#include "write.h"
#include "ioctl.h"
#include "uart.h"

void ReadThread(void* p)
{
	PMY_SERIAL_DEVICE_EXTENSION devExt;
	LARGE_INTEGER				time_to_wait;

	time_to_wait.QuadPart = -10000;
	devExt = (PMY_SERIAL_DEVICE_EXTENSION)p;

	while (devExt->mustExit == FALSE)
	{
		while (devExt->mustExit == FALSE && UartGetBit(devExt, LSR, LSR_DA) == 0)
		{
			  KeDelayExecutionThread(KernelMode, FALSE, &time_to_wait);
		}
		if (devExt->mustExit == FALSE)
		{
			WdfWaitLockAcquire(devExt->lock, NULL);
			if (devExt->readBufferLength >= MY_SERIAL_READ_BUFFER_MAX_LEN)
				devExt->readBufferLength = 0;
			devExt->readBuffer[devExt->readBufferLength++] = UartReadRegister(devExt, RBR);
			WdfWaitLockRelease(devExt->lock);
		}
	}
	PsTerminateSystemThread(STATUS_SUCCESS);
}

/*++
EvtDeviceAdd callback
--------------------
Appelée lorsque le PnP manager détecte un périphérique
qui correspond à un des ID matériel supporté par le driver.
Ces IDs sont définis dans le .inf du driver.
--------------------
IRQL = PASSIVE_LEVEL
--*/
NTSTATUS
EvtDeviceAdd(
    IN WDFDRIVER       Driver,
    IN PWDFDEVICE_INIT DeviceInit
    )
{
	NTSTATUS						status;
	static ULONG					currentInstance = 0;
	WDF_PNPPOWER_EVENT_CALLBACKS	pnpPowerCallbacks;
	WDFDEVICE						device;
	WDF_IO_QUEUE_CONFIG				queueConfig;
    WDFQUEUE						defaultqueue;
	PULONG							countSoFar;
	WDF_OBJECT_ATTRIBUTES			attributes;
	PMY_SERIAL_DEVICE_EXTENSION		devExt;
	OBJECT_ATTRIBUTES				ObjectAttributes;

	/*++
	Declare et initialise une UNICODE_STRING
	--*/
	DECLARE_UNICODE_STRING_SIZE(deviceName, 20);
	DECLARE_UNICODE_STRING_SIZE(symbolicLinkName, 50);

    UNREFERENCED_PARAMETER(Driver);

	/*++
	La macro PAGED_CODE() indique que le code qui suit dans cette fonction
	peut etre paginé (sur le disque, et non en mémoire physique)
	Permet au compilateur et aux outils de debug de détecter une erreur
	si le code tente d'apeller, par exemple, une fonction nécessitant
	un accès à des données situées obligatoirement en mémoire physique (pas de page_fault possible).
	--*/
	PAGED_CODE();

	KdPrint((DRIVER_NAME "--> EvtDeviceAdd\n"));

	/*++
	Récupere le nombre actuel de ports série enregistrés
	sur le système.
	--*/
	countSoFar = &IoGetConfigurationInformation()->SerialCount;

	/*++
	Construit une chaine de caractere unicode pour nommer le device.
	RtlUnicodeStringPrintf est définie dans <ntstrsafe.h>
	--*/
	status = RtlUnicodeStringPrintf(&deviceName, L"%ws%d",
                                L"\\Device\\Serial",
                                *countSoFar);
    if (!NT_SUCCESS(status))
	{
		KdPrint((DRIVER_NAME "Impossible de formater la string deviceName [0x%x]\n", status));
		return status;
	}
    status = WdfDeviceInitAssignName(DeviceInit, &deviceName);
    if (!NT_SUCCESS(status))
	{
		KdPrint((DRIVER_NAME "Impossible d'assigner le nom de device [0x%x]\n", status));
        return status;
	}
    WdfDeviceInitSetExclusive(DeviceInit, TRUE);
	/*++
	WdfDeviceInitSetDeviceType indique le type de péripherique géré.
	L'appel à cette fonction se traduit par la modification du boost de priorité
	appliqué lors de la completion d'un requete d'I/O, ce qui permet d'augmenter
	la priorité du thread qui a envoyé la requete.
	Les niveaux d'augmentation sont des constantes définies dans <wdm.h>
	--*/
    WdfDeviceInitSetDeviceType(DeviceInit, FILE_DEVICE_SERIAL_PORT);
	KdPrint((DRIVER_NAME "Device nommé, mode exclusif, FILE_DEVICE_SERIAL_PORT\n"));

	/*++
	Initialisation des callbacks PnP et gestion d'energie.
	Ces callbacks seront appelées par le framework lors des evénements correspondants.
	--*/
	WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
    pnpPowerCallbacks.EvtDevicePrepareHardware = EvtPrepareHardware;
    pnpPowerCallbacks.EvtDeviceReleaseHardware = EvtReleaseHardware;

	WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);

	/*++
	Création du device en lui meme, en lui associant une structure de données.
	--*/
	WDF_OBJECT_ATTRIBUTES_INIT_CONTEXT_TYPE(&attributes, MY_SERIAL_DEVICE_EXTENSION);
	attributes.EvtCleanupCallback = EvtDeviceContextCleanup;

	status = WdfDeviceCreate(&DeviceInit, &attributes, &device);
    if (!NT_SUCCESS(status))
	{
        KdPrint((DRIVER_NAME "WdfDeviceCreate failed 0x%x\n", status));
        return status;
    }

	devExt = MySerialGetDeviceExtension(device);
	devExt->hasIncrementedIoSerialCount = FALSE;
	devExt->readBufferLength = 0;
	devExt->mustExit = FALSE;

	/*++
	Initialisation de la queue par defaut, qui va recevoir toutes les
	requetes non gérees explicitement ailleurs.
	--*/
	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&queueConfig, WdfIoQueueDispatchParallel);
    queueConfig.EvtIoRead   = EvtIoRead;
    queueConfig.EvtIoWrite  = EvtIoWrite;
	queueConfig.EvtIoDeviceControl = EvtIoIoctl;
	queueConfig.EvtIoDefault = EvtIoDefault;


	status = WdfIoQueueCreate(device, &queueConfig, WDF_NO_OBJECT_ATTRIBUTES, &defaultqueue);
    if (!NT_SUCCESS(status))
	{
        KdPrint((DRIVER_NAME "WdfIoQueueCreate failed 0x%xn", status));
        return status;
    }


	/*++
	Crée un lien symbolique vers le device.
	TODO: déterminer dynamiquement via le registre
	--*/
	status = RtlUnicodeStringPrintf(&symbolicLinkName,
                                    L"%ws%ws",
                                    L"\\DosDevices\\",
                                    L"COM1");
	if (!NT_SUCCESS(status))
	{
		KdPrint((DRIVER_NAME "Impossible de formater la string symbolicLinkName [0x%x]\n", status));
		return status;
	}
	status = WdfDeviceCreateSymbolicLink(device, &symbolicLinkName);
    if (!NT_SUCCESS(status))
	{
      KdPrint(("Impossible de creer le lien symbolique.\n"));
	  return status;
    }

	/*++
	Renseigner le lien symbolique dans la base de registre
	--*/
	status = RtlWriteRegistryValue(RTL_REGISTRY_DEVICEMAP, L"SERIALCOMM",
                                   deviceName.Buffer,
                                   REG_SZ,
                                   L"COM1",
                                   deviceName.Length);
	if (!NT_SUCCESS(status))
	{
      KdPrint(("Impossible de renseigner la clé de registre concernant le port COM.\n"));
	  return status;
    }

	/*++
	Notre device est maintenant crée,
	on incremente le compteur de port série sur le système.
	On décrementera la valeur lors de la destruction du device ( appel à EvtDeviceContextCleanup )
	--*/
    (*countSoFar)++;
	devExt->hasIncrementedIoSerialCount = TRUE;

	WdfWaitLockCreate(WDF_NO_OBJECT_ATTRIBUTES, &devExt->lock);
	InitializeObjectAttributes(&ObjectAttributes, NULL, OBJ_KERNEL_HANDLE, NULL, NULL);
	status = PsCreateSystemThread(&devExt->readThread, 0L , &ObjectAttributes, NULL, NULL,
		ReadThread, devExt);

	KdPrint((DRIVER_NAME "<-- EvtDeviceAdd status=0x%x\n", status));
	return status;
}

VOID
EvtDeviceContextCleanup(
    WDFDEVICE       Device
    )
{
	PMY_SERIAL_DEVICE_EXTENSION devExt;
	PULONG	countSoFar;

	devExt = MySerialGetDeviceExtension(Device);
	if (devExt->hasIncrementedIoSerialCount)
	{
		countSoFar = &IoGetConfigurationInformation()->SerialCount;
		(*countSoFar)--;
	}
	devExt->mustExit = TRUE;
}
