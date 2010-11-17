#include "wdf_hello.h"

#ifdef ALLOC_PRAGMA
#pragma alloc_text (INIT, DriverEntry)
#pragma alloc_text (PAGE, WdfHelloEvtDeviceAdd)
#endif

NTSTATUS
DriverEntry(
    IN PDRIVER_OBJECT  DriverObject,
    IN PUNICODE_STRING RegistryPath
    )
{
    NTSTATUS            status = STATUS_SUCCESS;
    WDF_DRIVER_CONFIG   config;

    KdPrint(("Hello WDF World.\n"));
    KdPrint(("Built %s %s\n", __DATE__, __TIME__));

	WDF_DRIVER_CONFIG_INIT(
        &config,
        WdfHelloEvtDeviceAdd
        );

    status = WdfDriverCreate(
        DriverObject,
        RegistryPath,
        WDF_NO_OBJECT_ATTRIBUTES,
        &config,
        WDF_NO_HANDLE
        );

    if (!NT_SUCCESS(status)) {
        KdPrint( ("WdfDriverCreate failed with status 0x%x\n", status));
    }
    return status;
}


NTSTATUS
WdfHelloEvtDeviceAdd(
    IN WDFDRIVER       Driver,
    IN PWDFDEVICE_INIT DeviceInit
    )
{
    NTSTATUS               status = STATUS_SUCCESS;

    UNREFERENCED_PARAMETER(Driver);
    PAGED_CODE();
    KdPrint(("WdfHelloEvtDeviceAdd called\n"));
    return status;
}
