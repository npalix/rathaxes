#include "driver.h"

#include <ntddser.h>

#include "ioctl.h"
#include "device.h"
#include "uart.h"

VOID EvtIoIoctl(
    IN WDFQUEUE		Queue,
    IN WDFREQUEST	Request,
    IN size_t		OutputBufferLength,
    IN size_t		InputBufferLength,
    IN ULONG		IoControlCode
    )
{
	NTSTATUS	status = STATUS_SUCCESS;
	PMY_SERIAL_DEVICE_EXTENSION pContext;
    PVOID		buffer;
    size_t		bufSize;



	KdPrint((DRIVER_NAME "-->EvtIoIoctl\n"));
	pContext = MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue));
	switch (IoControlCode)
	{
	case IOCTL_SERIAL_GET_BAUD_RATE:
		status = WdfRequestRetrieveOutputBuffer(Request, sizeof(ULONG), &buffer, &bufSize);
		if(!NT_SUCCESS(status))
		{
			KdPrint(("Could not get request memory buffer %X\n", status));
			break;
		}
		*((ULONG*)buffer) = 115200;
		WdfRequestCompleteWithInformation(Request, status, sizeof(ULONG));
		return;

	case IOCTL_SERIAL_GET_LINE_CONTROL:
		status = WdfRequestRetrieveOutputBuffer(Request, sizeof(SERIAL_LINE_CONTROL), &buffer, &bufSize);
		((SERIAL_LINE_CONTROL*)buffer)->StopBits = 1;
		((SERIAL_LINE_CONTROL*)buffer)->Parity = 0;
		((SERIAL_LINE_CONTROL*)buffer)->WordLength = 8;
		WdfRequestCompleteWithInformation(Request, status, sizeof(SERIAL_LINE_CONTROL));
		return;

	default:
		break;
	}

	WdfRequestComplete(Request, STATUS_NOT_IMPLEMENTED);
	KdPrint((DRIVER_NAME "<--EvtIoIoctl\n"));
}
