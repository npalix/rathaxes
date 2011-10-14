#include "driver.h"

#include "device.h"
#include "uart.h"


NTSTATUS
EvtPrepareHardware(
		WDFDEVICE Device,
		WDFCMRESLIST Resources,
		WDFCMRESLIST ResourcesTranslated
		)
{
	ULONG	i;
	PCM_PARTIAL_RESOURCE_DESCRIPTOR		descriptor;
	PMY_SERIAL_DEVICE_EXTENSION			devExt;

	KdPrint((DRIVER_NAME "-->EvtPrepareHardware\n"));
	
	devExt = MySerialGetDeviceExtension(Device);
	for (i = 0; i < WdfCmResourceListGetCount(ResourcesTranslated); i++)
	{
		descriptor = WdfCmResourceListGetDescriptor(ResourcesTranslated, i);
		if (!descriptor)
			return STATUS_DEVICE_CONFIGURATION_ERROR;
		switch (descriptor->Type)
		{
		case CmResourceTypePort:
            KdPrint((DRIVER_NAME "I/O Port: (%x) Length: (%d)\n",
                    descriptor->u.Port.Start.LowPart,
                    descriptor->u.Port.Length));
			devExt->portBase = descriptor->u.Port.Start.LowPart;
			break;
		default:
			break;
		}
	}
	UartInit(MySerialGetDeviceExtension(Device));
	KdPrint((DRIVER_NAME "<--EvtPrepareHardware\n"));
	return STATUS_SUCCESS;
}

NTSTATUS
EvtReleaseHardware(
    IN  WDFDEVICE Device,
    IN  WDFCMRESLIST ResourcesTranslated
    )
{
	KdPrint((DRIVER_NAME "-->EvtReleaseHardware\n"));
	KdPrint((DRIVER_NAME "<--EvtReleaseHardware\n"));
	return STATUS_SUCCESS;
}