#include "driver.h"

#include "uart.h"

void UartInit(PMY_SERIAL_DEVICE_EXTENSION pContext)
{
    UartSetBaudrate(pContext, 115200);
    UartSetMode(pContext, 8, 1, 0, 0);

    UartSetBit(pContext, MCR, MCR_DTR, 1);
    UartSetBit(pContext, MCR, MCR_RTS, 1);
}

void UartEnableInterrupts(PMY_SERIAL_DEVICE_EXTENSION pContext)
{
	/*
    WRITE_PORT_UCHAR((PUCHAR)(pContext->portBase + IER), (ERBFI | ETBEI));
    UartReadRegister(pContext, IIR);
    UartSetBit(pContext, MCR, OUT2, 1);
	*/
}

void UartWriteByte(PMY_SERIAL_DEVICE_EXTENSION pContext, UCHAR value)
{
    KdPrint((DRIVER_NAME "-->UartWriteByte [%c]\n", value));
    UartWriteRegister(pContext, THR, value);
    KdPrint((DRIVER_NAME "<--UartWriteByte\n"));
}

void UartWriteRegister(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg, UCHAR value)
{
    WRITE_PORT_UCHAR((PUCHAR)(pContext->portBase + reg), value);
}

UCHAR UartReadRegister(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg)
{
    return READ_PORT_UCHAR((PUCHAR)(pContext->portBase + reg));
}

void UartSetBit(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg, int bit, int value)
{
    UCHAR   prev_value;
	UCHAR	new_value;

    prev_value = READ_PORT_UCHAR((PUCHAR)(pContext->portBase + reg));
	new_value = (value) ? (prev_value | bit) : (prev_value & (~bit));
	WRITE_PORT_UCHAR((PUCHAR)(pContext->portBase + reg), new_value);
}

int UartGetBit(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg, int bit)
{
	UCHAR read;

	read = READ_PORT_UCHAR((PUCHAR)(pContext->portBase + reg));
    return (read & bit);
}

void UartSetBaudrate(PMY_SERIAL_DEVICE_EXTENSION pContext, int baudrate)
{
    int divisor;
    UCHAR dummy;

	//Disable Interrupts
    UartSetBit(pContext, MCR, MCR_AO2, 0);

    divisor = BASE_BAUDRATE / baudrate;

    UartSetBit(pContext, LCR, LCR_DLAB, 1);

    WRITE_PORT_UCHAR((PUCHAR)(pContext->portBase + DLL), (UCHAR)divisor);
    WRITE_PORT_UCHAR((PUCHAR)(pContext->portBase + DLM), (divisor >> 8));

	UartSetBit(pContext, LCR, LCR_DLAB, 0);
}


int UartSetMode(PMY_SERIAL_DEVICE_EXTENSION pContext, int dataBits, int stopBits, int parity, int odd)
{
    int dataBitsUart;
	int stopBitsUart;
	int parityUart;
    UCHAR registerValue;

    if((dataBits >= 5) && (dataBits <= 8))
	{
        dataBitsUart = (dataBits - 5);
    }
    else
		return - 1;

    if((stopBits >= 1) && (stopBits <= 2))
		stopBitsUart = (stopBits == 1) ? 0 : LCR_SB;
    else
		return - 2;

    if(parity)
		parityUart = (odd) ? LCR_PAR : LCR_PAR + LCR_EVN;
    else
		parityUart = 0;

    registerValue = dataBitsUart + stopBitsUart + parityUart;
    UartWriteRegister(pContext, LCR, registerValue);
	return 0;
}
