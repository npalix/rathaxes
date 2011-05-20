#include <ntddk.h>

void DriverUnload(PDRIVER_OBJECT pDriverObject)
{
        DbgPrint("Driver Unloading");
}

NTSTATUS DriverEntry(PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	DriverObject->DriverUnload = DriverUnload;
	DbgPrint("Goodbye, World\n");
	return STATUS_SUCCESS;
}
