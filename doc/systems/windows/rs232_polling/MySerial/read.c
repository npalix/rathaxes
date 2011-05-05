#include "driver.h"

#include "device.h"
#include "uart.h"

VOID
EvtIoRead(
    IN WDFQUEUE         Queue,
    IN WDFREQUEST       Request,
    IN size_t           Length
    )
{
	WDFMEMORY						memory;
	NTSTATUS						status;
	PMY_SERIAL_DEVICE_EXTENSION		pContext;
	size_t							finalLength;

	KdPrint((DRIVER_NAME "-->EvtIoRead\n"));
	status = WdfRequestRetrieveOutputMemory(Request, &memory);
	if(NT_SUCCESS(status))
	{
		pContext = MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue));
		if (pContext->readBuffer)
		{
			WdfWaitLockAcquire(pContext->lock, NULL);
			finalLength = (pContext->readBufferLength >= Length) ? (Length) : (pContext->readBufferLength);
			status = WdfMemoryCopyFromBuffer(memory,
											0,
											pContext->readBuffer,
											finalLength);	
			pContext->readBufferLength -= finalLength;
			memmove(pContext->readBuffer, pContext->readBuffer + finalLength, MY_SERIAL_READ_BUFFER_MAX_LEN - finalLength);
			WdfWaitLockRelease(pContext->lock);
			KdPrint((DRIVER_NAME "Read %d bytes\n", finalLength));
			WdfRequestCompleteWithInformation(Request, status, finalLength);
		}
		else
		{
			WdfRequestCompleteWithInformation(Request, status, 0L);
		}
	}
	else
	{
		WdfRequestComplete(Request, status);
	}
	KdPrint((DRIVER_NAME "<--EvtIoRead\n"));
}


VOID
EvtIoDefault(
	IN WDFQUEUE  Queue,
	IN WDFREQUEST  Request
	)
{
	KdPrint((DRIVER_NAME "-->EvtIoDefault\n"));

	UartWriteByte(MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue)), 'B');
	//while ( UartGetBit(pContext, 5, 0x20) == 0 ) {}
	//UartWriteByte(MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue)), 'i');
	//while ( UartGetBit(pContext, 5, 0x20) == 0 ) {}
	//UartWriteByte(MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue)), 't');
	//while ( UartGetBit(pContext, 5, 0x20) == 0 ) {}
	//UartWriteByte(MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue)), 'e');
	//while ( UartGetBit(pContext, 5, 0x20) == 0 ) {}
	//UartWriteByte(MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue)), '\r');
	//while ( UartGetBit(pContext, 5, 0x20) == 0 ) {}
	//UartWriteByte(MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue)), '\n');
	//while ( UartGetBit(pContext, 5, 0x20) == 0 ) {}

	WdfRequestComplete(Request, STATUS_NOT_IMPLEMENTED);
	KdPrint((DRIVER_NAME "<--EvtIoDefault\n"));
}