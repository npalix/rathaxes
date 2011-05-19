/*++
uart.h
Contains defines for UART registers
--*/

#ifndef UART_H_
# define UART_H_

#include "device.h"

#define BASE_BAUDRATE	115200

/*++
UART registers when DLAB = 0 for read operations
--*/
# define RBR		0	/* Receiver buffer */

/*++
UART registers when DLAB = 0 for write operations
--*/
# define THR		0	/* Transmitter holding */

/*++
UART registers when DLAB = 0 for read AND write operations
--*/
# define IER		1	/* Interrupt enable */

/*++
UART registers when DLAB = 1 for read AND write operations
-*/
# define DLL		0	/* Divisor latch LSB */
# define DLM		1	/* Divisor latch MSB */

/*++
UART register DLAB independant for read operations
--*/
# define IIR		2	/* Interrupt identification */
# define LSR		5	/* Line status */
# define MSR		6	/* Modem status */

/*++
UART register DLAB independant for write operations
--*/
# define FCR		2	/* Fifo control */

/*++
UART register DLAB independant for read AND write operations
--*/
# define LCR		3	/* Line control */
# define MCR		4	/* Modem control */
# define SCR		7	/* Scratch */

/*++
DLAB Bit (On LCR)
--*/
# define LCR_SB		(1 << 2)	/*Stop bits*/
# define LCR_PAR	0x08		/*Parity enabled*/
# define LCR_EVN	0x10		/*Even parity*/
# define LCR_DLAB	(1 << 7)	/*DLAB switch*/

/*++
LSR Bits
--*/
# define LSR_DA		(1 << 0)	/*Data Avaible*/
# define LSR_ETHR	(1 << 5)	/*Empty Transmitter Holding Register*/

/*++
MCR (Modem Control) Bits
--*/
# define MCR_DTR		(1 << 0)	/*Data Terminal Ready*/
# define MCR_RTS		(1 << 1)	/*Request To Send*/
# define MCR_AO2		(1 << 3)	/*Auxiliary Output 2*/

/*++
IER (Interrupt Enable Register) Bits
--*/
#define IER_RDA			(1 << 0)    /*Received Data Available*/
#define IER_THRE		(1 << 1)	/*Transmitter Holding Register Empty*/
#define IER_RLS			(1 << 2)    /*Receiver Line Status*/
#define IER_MSR			(1 << 3)    /*Modem Status Register*/


void UartInit(PMY_SERIAL_DEVICE_EXTENSION pContext);
void UartEnableInterrupts(PMY_SERIAL_DEVICE_EXTENSION pContext);
void UartWriteByte(PMY_SERIAL_DEVICE_EXTENSION pContext, UCHAR value);
void UartWriteRegister(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg, UCHAR value);
UCHAR UartReadRegister(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg);
void UartSetBit(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg, int bit, int value);
int UartGetBit(PMY_SERIAL_DEVICE_EXTENSION pContext, int reg, int bit);
void UartSetBaudrate(PMY_SERIAL_DEVICE_EXTENSION pContext, int baudrate);
int UartSetMode(PMY_SERIAL_DEVICE_EXTENSION pContext, int dataBits, int stopBits, int parity, int odd);

#endif /* ! UART_H_ */
