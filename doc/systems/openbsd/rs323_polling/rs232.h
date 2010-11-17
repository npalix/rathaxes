/*
**      Copyright (c) 2008, Amsallem David <filth@rathaxes.org>
**
**      Permission to use, copy, modify, and/or distribute this software for any
**      purpose with or without fee is hereby granted, provided that the above
**      copyright notice and this permission notice appear in all copies.
**
**      THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
**      WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
**      MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
**      ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
**      WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN AN
**      ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT OF
**      OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
*/

#include <machine/bus.h>
#include <sys/ioctl.h>

#ifndef RS232_H__
# define RS232_H__

/******************************************/
/* UART register to port conversion table */
/******************************************/

# define UART_RBR                 0
# define UART_THR                 0
# define UART_DLL                 0
# define UART_IER                 1
# define UART_DLM                 1
# define UART_IIR                 2
# define UART_FCR                 2
# define UART_LCR                 3
# define UART_MCR                 4
# define UART_LSR                 5
# define UART_MSR                 6
# define UART_SCR                 7

/*******************/
/*    LSR masks    */
/*******************/

# define LSR_DATA_READY               0x01
# define LSR_EMPTY_TRANSMITTER        0x20

/*******************/
/*    FCR masks    */
/*******************/

# define FCR_ENABLE_FIFO              0xC7

/*******************/
/*    IER masks    */
/*******************/

# define IER_DISABLE_INT              0x00

/*******************/
/*    LCR masks    */
/*******************/

# define LCR_ONE_STOP_BIT             0x00
# define LCR_TWO_STOP_BITS            0x04
# define LCR_STOP_BIT                 0x04
# define LCR_WORD_5                   0x00
# define LCR_WORD_6                   0x01
# define LCR_WORD_7                   0x02
# define LCR_WORD_8                   0x03
# define LCR_WORD                     0x03
# define LCR_NO_PARITY                0x00
# define LCR_ODD_PARITY               0x08
# define LCR_EVEN_PARITY              0x18
# define LCR_PARITY                   0x38
# define LCR_DLAB                     0x80

/*******************/
/*    MCR masks    */
/*******************/

# define ENABLE_MODEM                 0x0B

/*******************/
/*    DLL masks    */
/*******************/

# define DLL_9600                     0x0C
# define DLL_115200                   0x01

/*******************/
/*    DLM masks    */
/*******************/
# define DLM_9600                     0x00
# define DLM_115200                   0x00

/*****************/
/* UART baudrare */
/*****************/ 

# define UART_BAUDRATE            115200

/*---------------------------------------------*/

/********************/
/* Number of device */
/********************/ 

# define NB_DEVS 4

/*****************/
/* IOCTL command */
/*****************/

# define SET_BAUDRATE _IOW(42, 0, int)
# define SET_PARITY _IOW(42, 1, int)
# define SET_STOPBIT _IOW(42, 2, int)
# define SET_WORD _IOW(42, 3, int)

# define GET_BAUDRATE _IOR(42, 10, int)
# define GET_LCRSTATUS _IOR(42, 11, int)

/********************/
/* Device context   */
/********************/

typedef struct    rs232_device
{
  bus_addr_t      io_base;
  char            *name;
  bus_space_tag_t io_tag;
  int             baudrate;
  char            busy;
  char            reg_rbr;
  char            reg_thr;
  char            reg_ier;
  char            reg_iir;
  char            reg_fcr;
  char            reg_lcr;
  char            reg_mcr;
  char            reg_lsr;
  char            reg_msr;
  char            reg_scr;
}                 rs232_device_t;

# define REG_SET(reg, val) ((reg) |= (val))
# define REG_CLR(reg, val) ((reg) &= ~(val))
# define REG_ISSET(reg, val) ((reg) & (val))

//void    rs232_uartInit(void);
int     rs232_open(dev_t dev, int oflags, int devtype, struct proc *p);
int     rs232_close(dev_t dev, int fflag, int devtype, struct proc *p);
int     rs232_read(dev_t dev, struct uio *uio, int ioflag);
int     rs232_write(dev_t dev, struct uio *uio, int ioflag);
int     rs232_ioctl(dev_t dev, u_long cmd, caddr_t data, int fflag, struct proc *p);
int     rs232_init(void);

#endif
