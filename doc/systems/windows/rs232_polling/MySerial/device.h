#ifndef DEVICE_H_
# define DEVICE_H_

VOID
EvtDeviceContextCleanup (
    WDFDEVICE       Device
    );

VOID
EvtIoQueueContextDestroy(
    WDFOBJECT Object
);

# define MY_SERIAL_READ_BUFFER_MAX_LEN	4096

typedef struct _MY_SERIAL_DEVICE_EXTENSION
{
	BOOLEAN		hasIncrementedIoSerialCount;
	ULONG		portBase;
	char		readBuffer[MY_SERIAL_READ_BUFFER_MAX_LEN];
	size_t		readBufferLength;
	HANDLE		readThread;
	BOOLEAN		mustExit;
	WDFWAITLOCK	lock;

} MY_SERIAL_DEVICE_EXTENSION,*PMY_SERIAL_DEVICE_EXTENSION;

WDF_DECLARE_CONTEXT_TYPE_WITH_NAME(MY_SERIAL_DEVICE_EXTENSION, MySerialGetDeviceExtension)


#endif //!DEVICE_H_
