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

#include <sys/param.h>
#include <sys/conf.h>
#include <sys/exec.h>
#include <sys/systm.h>
#include <sys/lkm.h>
#include <sys/malloc.h>
#include <machine/bus.h>
#include "rs232.h"

rs232_device_t  gl_devs[4] =
{
  {0x3f8, "PCCOM_1", I386_BUS_SPACE_IO, 0, 0, 0, IER_DISABLE_INT, 0, FCR_ENABLE_FIFO, LCR_WORD_8 | LCR_ONE_STOP_BIT | LCR_NO_PARITY, 0, 0, 0, 0},
  {0x3e8, "PCCOM_2", I386_BUS_SPACE_IO, 0, 0, 0, IER_DISABLE_INT, 0, FCR_ENABLE_FIFO, LCR_WORD_8 | LCR_ONE_STOP_BIT | LCR_NO_PARITY, 0, 0, 0, 0},
  {0x2f8, "PCCOM_3", I386_BUS_SPACE_IO, 0, 0, 0, IER_DISABLE_INT, 0, FCR_ENABLE_FIFO, LCR_WORD_8 | LCR_ONE_STOP_BIT | LCR_NO_PARITY, 0, 0, 0, 0},
  {0x2e8, "PCCOM_4", I386_BUS_SPACE_IO, 0, 0, 0, IER_DISABLE_INT, 0, FCR_ENABLE_FIFO, LCR_WORD_8 | LCR_ONE_STOP_BIT | LCR_NO_PARITY, 0, 0, 0, 0}
};

int rs232_open(dev_t dev, int oflags, int devtype, struct proc *p)
{
  rs232_device_t  *dev_current;
  int             minor;
  int             divisor;

  printf("open\n");
  minor = minor(dev);
  if (minor >= NB_DEVS)
    return ENXIO;
  dev_current = &gl_devs[minor];
  if (dev_current->busy)
    return EBUSY;
  dev_current->busy = 1;
  divisor =  dev_current->baudrate / UART_BAUDRATE;
  /* LCR word length, stop bit and parity initialization */
  bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
  /* FCR enable FIFO initialization */
  bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_FCR, dev_current->reg_fcr);
  /* IER disable interrupts initialization */
  bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_IER, dev_current->reg_ier);
  /* LCR DLAB switch on */
  REG_SET(dev_current->reg_lcr, LCR_DLAB);
  bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
  /* DLL baudrate initialization */
  bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_DLL, divisor & 0xff);
  /* DLM baudrate initialization */
  bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_DLM, divisor >> 8);
  /* LCR DLAB switch off */
  REG_CLR(dev_current->reg_lcr, LCR_DLAB);
  bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
  return (0);
}

int rs232_close(dev_t dev, int fflag, int devtype, struct proc *p)
{
  rs232_device_t  *dev_current;
  int             minor;

  printf("close\n");
  minor = minor(dev);
  if (minor >= NB_DEVS)
    return ENXIO;
  dev_current = &gl_devs[minor];
  dev_current->busy = 0;
  return (0);
}

int rs232_read(dev_t dev, struct uio *uio, int ioflag)
{
//  rs232_device_t  *dev_current;
//  int             minor;
//  char            status;
//  char            *buff;
//  int             len;
//  int             i;

  printf("read\n");
//  /* Check if data should be read into the space */
//  if (uio->uio_rw != UIO_READ)
//    return EPERM;
//  /* Check if the device is implemented */
//  minor = minor(dev);
//  if (minor >= NB_DEVS)
//    return ENXIO;
//  dev_current = &gl_devs[minor];
//  /* Test if the device is busy */
//  if (dev_current->busy == 0)
//    return EBUSY;
//  /* FIXME : Patch for test, the len will be always uio_resid */
//#define PATCH_TEST_LEN 1
//  if ((buff = malloc(PATCH_TEST_LEN * sizeof(*buff), M_TEMP, M_NOWAIT)) == NULL)
//    return ENOMEM;
//  len = (uio->uio_resid < PATCH_TEST_LEN) ? uio->uio_resid : PATCH_TEST_LEN;
//  /* Will read 'len' char asked by the user space */
//  for (i = 0; i < len; i++)
//  {
//    /* block until a character becomes available */
//    while (!REG_ISSET(status = bus_space_read_1(dev_current->io_tag, dev_current->io_base, UART_LSR), LSR_DATA_READY))
//      continue;
//    /* read data available */
//    buff[i] = bus_space_read_1(dev_current->io_tag, dev_current->io_base, UART_RBR);
//  }
//  /* move data from kernel space to user space */
//  uiomove(buff, len, uio);
  return (0);
}

int rs232_write(dev_t dev, struct uio *uio, int ioflag)
{
//  rs232_device_t  *dev_current;
//  int             minor;
//  char            *buff;
//  char            status;
//  int             i;

  printf("write\n");
//  /* Check if data should be write from the space */
//  if (uio->uio_rw != UIO_WRITE)
//    return EPERM;
//  /* Check if the device is implemented */
//  minor = minor(dev);
//  if (minor >= NB_DEVS)
//    return ENXIO;
//  dev_current = &gl_devs[minor];
//  /* Test if the device is busy */
//  if (dev_current->busy == 0)
//    return EBUSY;
//  if ((buff = malloc(uio->uio_resid * sizeof(*buff), M_TEMP, M_NOWAIT)) == NULL)
//    return ENOMEM;
//  /* move data from user space to kernel space */
//  if (uiomove(buff, uio->uio_resid, uio) != 0)
//    return EFAULT;
//  /* Will write 'len' char asked by the user space */
//  for(i = 0; i < uio->uio_resid; i++)
//  {
//    /* block until the transmitter register is not empty */
//    while(!REG_ISSET(status = bus_space_read_1(dev_current->io_tag, dev_current->io_base, UART_LSR), LSR_EMPTY_TRANSMITTER))
//      continue;
//    /* write data on the UART */
//    bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_THR, buff[i]);
//  }
//  free(buff, M_TEMP);
  return (0);
}

int rs232_ioctl(dev_t dev, u_long cmd, caddr_t data, int fflag, struct proc *p)
{
//  rs232_device_t  *dev_current;
//  int             minor;

  printf("ioctl\n");
//  minor = minor(dev);
//  if (minor >= NB_DEVS)
//    return ENXIO;
//  dev_current = &gl_devs[minor];
//  switch(cmd)
//  {
//    case SET_BAUDRATE:
//      dev_current->baudrate = *data;
//      int divisor =  dev_current->baudrate / UART_BAUDRATE;
//      if (!REG_ISSET(dev_current->reg_lcr, LCR_DLAB))
//      {
//        /* LCR DLAB switch on */
//        REG_SET(dev_current->reg_lcr, LCR_DLAB);
//        bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
//      }
//      /* DLL baudrate initialization */
//      bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_DLL, divisor & 0xff);
//      /* DLM baudrate initialization */
//      bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_DLM, divisor >> 8);
//      if (REG_ISSET(dev_current->reg_lcr, LCR_DLAB))
//      {
//        /* LCR DLAB switch off */
//        REG_CLR(dev_current->reg_lcr, LCR_DLAB);
//        bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
//      }
//      break;
//    case SET_PARITY:
//      REG_CLR(dev_current->reg_lcr, LCR_PARITY);
//      REG_SET(dev_current->reg_lcr, *data);
//      bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
//      break;
//    case SET_STOPBIT:
//      REG_CLR(dev_current->reg_lcr, LCR_STOP_BIT);
//      REG_SET(dev_current->reg_lcr, *data);
//      bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
//      break;
//    case SET_WORD:
//      REG_CLR(dev_current->reg_lcr, LCR_WORD);
//      REG_SET(dev_current->reg_lcr, *data);
//      bus_space_write_1(dev_current->io_tag, dev_current->io_base, UART_LCR, dev_current->reg_lcr);
//      break;
//    case GET_BAUDRATE:
//      *data = dev_current->baudrate;
//      break;
//    case GET_LCRSTATUS:
//      *data = dev_current->reg_lcr;
//      break;
//    default:
//      return ENOTTY;
//      break;
//  }
  return (0);
}
