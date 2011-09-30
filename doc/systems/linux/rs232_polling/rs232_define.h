#ifndef __RS232_DEFINE_H__
#define __RS232_DEFINE_H__

// ports offset from base port
#define RBR 0   // read buffer
#define THR 0   // write buffer
#define DLL 0   // latch LSB
#define IER 1   // interrupt enable
#define DLM 1   // latch MSB
#define IIR 2   // interupt identification
#define FCR 2   // fifo control
#define LCR 3   // line control
#define MCR 4   // modem control
#define LSR 5   // line status
#define MSR 6   // modem status
#define SCR 7   // scratch



// bits MASK
// on LCR
#define LCR_WDL     0x03
#define LCR_STP     0x04
#define LCR_PAR     0x38
#define LCR_BRK     0x40
#define LCR_DLAB    0x80

// on LSR
#define LSR_DATA_AVAILABLE      0x01
#define LSR_OVERRUN_ERR         0x02
#define LSR_PARITY_ERR          0x04
#define LSR_FRAMING_ERR         0x08
#define LSR_BREAK_SIG_RCV       0x10
#define LSR_THR_EMPTY           0x20
#define LSR_THR_IDL             0x40
#define LSR_FIFO_ERR            0x80

//  on FCR
#define FCR_FIFO_AVALABILITY    0x01
#define FCR_RECIEVE             0x02
#define FCR_TRANS               0x04
#define FCR_DMA_MODE            0x08


// bits value to set
// LCR register
#define LCR_5_BIT_WORD          0X00
#define LCR_6_BIT_WORD          0X01
#define LCR_7_BIT_WORD          0X02
#define LCR_8_BIT_WORD          0X03
#define LCR_1_STOP_BIT          0X00
#define LCR_2_STOP_BIT          0x04
#define LCR_NO_PARITY           0x00
#define LCR_ODD_PARITY          0x08
#define LCR_EVEN_PARITY         0x18
#define LCR_BREAK_SIG_ENABLE    0x40
#define LCR_BREAK_SIG_DISABLE   0x00
#define LCR_DLAB_LATCH_MODE     0x80
#define LCR_DLAB_RW_MODE        0x00
// IER interrupt enable register
#define IER_NONE                0x00
// FCR
#define FCR_DISABLE             0x00
#define FCR_ENABLE              0x01
#define FCR_CLEAR_RCV           0x02
#define FCR_CLEAR_TR            0x04
// LATCH registers
#define DLL_9600                0x0c
#define DLM_9600                0x00
#define DLL_115200              0x01
#define DLM_115200              0x00
// for userside ioctl
#define DIVISOR_115200          0x0001
#define DIVISOR_9600            0x000c
#define DIVISOR_38400           0x0003


//  IOCTL relative defines
#define RS_IOCTL_MAGIC          0xf5
#define RS_BAUDRATESET          _IOW(RS_IOCTL_MAGIC, 1, int)
#define RS_BAUDRATEGET          _IO(RS_IOCTL_MAGIC, 2)
#define RS_WORDLENGTHSET        _IOW(RS_IOCTL_MAGIC, 3, int)
#define RS_STOPBITSET           _IOW(RS_IOCTL_MAGIC, 4, int)
#define RS_PARITYSET            _IOW(RS_IOCTL_MAGIC, 5, int)
#define RS_CONFSET              _IOW(RS_IOCTL_MAGIC, 6, rsconf)

// structure for setting all parameter at once
typedef struct rs232_conf_t
{
    int     wl;
    int     div;
    char    par;
}rsconf;

#endif  /* __RS232_DEFINE_H__ */
