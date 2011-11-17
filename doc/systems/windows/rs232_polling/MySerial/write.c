#include "driver.h"

#include "device.h"
#include "uart.h"

VOID
EvtIoWrite(
    IN WDFQUEUE         Queue,
    IN WDFREQUEST       Request,
    IN size_t           Length
    )
{
	WDFMEMORY					memory;
	NTSTATUS					status;
	PMY_SERIAL_DEVICE_EXTENSION	pContext;
	PUCHAR						buffer;
	size_t						buf_size;
	size_t						i = 0;

	KdPrint((DRIVER_NAME "-->EvtIoWrite\n"));
	pContext = MySerialGetDeviceExtension(WdfIoQueueGetDevice(Queue));
	status = WdfRequestRetrieveInputMemory(Request, &memory);
	if(!NT_SUCCESS(status))
	{
        KdPrint((DRIVER_NAME "EvtIoWrite Could not get request memory buffer 0x%x\n",
                 status));
        WdfRequestComplete(Request, status);
		KdPrint((DRIVER_NAME "<-- EvtDeviceIoWrite\n"));
        return;
    }
	buffer = WdfMemoryGetBuffer(memory, &buf_size);
	KdPrint((DRIVER_NAME "Sending a buffer of %d bytes\n", buf_size));
	while (i < buf_size)
	{
		while (UartGetBit(pContext, LSR, LSR_ETHR) == 0 ) {}
		UartWriteByte(pContext, buffer[i]);
		i++;
	}
	WdfRequestCompleteWithInformation(Request, status, i);
	KdPrint((DRIVER_NAME "<--EvtIoWrite\n"));
}
