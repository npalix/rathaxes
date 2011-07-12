#include <ntddk.h>
#include <wdf.h>
#include <initguid.h>

// {E20808D9-5E08-4a2a-80A1-84DA0B2180AB}
DEFINE_GUID(GUID_DEVCLASS_RATHAXES, 
0xe20808d9, 0x5e08, 0x4a2a, 0x80, 0xa1, 0x84, 0xda, 0xb, 0x21, 0x80, 0xab);

// {1ADB9D41-6FE5-432e-A5FF-9B6911A150A4}
DEFINE_GUID(GUID_DEVINTERFACE_RATHAXES_SERIAL,
0x1adb9d41, 0x6fe5, 0x432e, 0xa5, 0xff, 0x9b, 0x69, 0x11, 0xa1, 0x50, 0xa4);

typedef signed int DWORD;
typedef signed int WORD;
typedef signed char BYTE;
typedef signed char BIT;
typedef signed int t_bit;
typedef unsigned char t_register;
typedef unsigned int t_iobase;
enum e_ier 
{
	ier_DATA_AVAILABLE_DATA_AVAILABLE = 1,
	ier_DATA_AVAILABLE_DATA_AVAILABLE_VALUE = 1,
	ier_DATA_AVAILABLE_DATA_AVAILABLE_MASK = 1,
	ier_TRANSMITTER_EMPTY_TRANSMITTER_EMPTY = 1,
	ier_TRANSMITTER_EMPTY_TRANSMITTER_EMPTY_VALUE = 2,
	ier_TRANSMITTER_EMPTY_TRANSMITTER_EMPTY_MASK = 2,
	ier_LINE_STATUS_CHANGE_LINE_STATUS_CHANGE = 1,
	ier_LINE_STATUS_CHANGE_LINE_STATUS_CHANGE_VALUE = 4,
	ier_LINE_STATUS_CHANGE_LINE_STATUS_CHANGE_MASK = 4,
	ier_MODEM_STATUS_CHANGE_MODEM_STATUS_CHANGE = 1,
	ier_MODEM_STATUS_CHANGE_MODEM_STATUS_CHANGE_VALUE = 8,
	ier_MODEM_STATUS_CHANGE_MODEM_STATUS_CHANGE_MASK = 8,
	ier_SLEEP_MODE_SLEEP_MODE = 1,
	ier_SLEEP_MODE_SLEEP_MODE_VALUE = 16,
	ier_SLEEP_MODE_SLEEP_MODE_MASK = 16,
	ier_LOW_POWER_MODE_LOW_POWER_MODE = 1,
	ier_LOW_POWER_MODE_LOW_POWER_MODE_VALUE = 32,
	ier_LOW_POWER_MODE_LOW_POWER_MODE_MASK = 32
};
enum e_lcr 
{
	lcr_WORD_LENGH__5BITS = 0,
	lcr_WORD_LENGH__5BITS_VALUE = 0,
	lcr_WORD_LENGH__5BITS_MASK = 3,
	lcr_WORD_LENGH__6BITS = 1,
	lcr_WORD_LENGH__6BITS_VALUE = 1,
	lcr_WORD_LENGH__6BITS_MASK = 3,
	lcr_WORD_LENGH__7BITS = 2,
	lcr_WORD_LENGH__7BITS_VALUE = 2,
	lcr_WORD_LENGH__7BITS_MASK = 3,
	lcr_WORD_LENGH__8BITS = 3,
	lcr_WORD_LENGH__8BITS_VALUE = 3,
	lcr_WORD_LENGH__8BITS_MASK = 3,
	lcr_STOP_BITS__1STOP_BITS = 0,
	lcr_STOP_BITS__1STOP_BITS_VALUE = 0,
	lcr_STOP_BITS__1STOP_BITS_MASK = 4,
	lcr_STOP_BITS__2STOP_BITS = 1,
	lcr_STOP_BITS__2STOP_BITS_VALUE = 4,
	lcr_STOP_BITS__2STOP_BITS_MASK = 4,
	lcr_PARITY_TYPE_NONE = 0,
	lcr_PARITY_TYPE_NONE_VALUE = 0,
	lcr_PARITY_TYPE_NONE_MASK = 56,
	lcr_PARITY_TYPE_ODD = 1,
	lcr_PARITY_TYPE_ODD_VALUE = 8,
	lcr_PARITY_TYPE_ODD_MASK = 56,
	lcr_PARITY_TYPE_EVEN = 3,
	lcr_PARITY_TYPE_EVEN_VALUE = 24,
	lcr_PARITY_TYPE_EVEN_MASK = 56,
	lcr_PARITY_TYPE_HIGH = 5,
	lcr_PARITY_TYPE_HIGH_VALUE = 40,
	lcr_PARITY_TYPE_HIGH_MASK = 56,
	lcr_PARITY_TYPE_LOW = 7,
	lcr_PARITY_TYPE_LOW_VALUE = 56,
	lcr_PARITY_TYPE_LOW_MASK = 56,
	lcr_BREAK_SIGNAL_DISABLE = 0,
	lcr_BREAK_SIGNAL_DISABLE_VALUE = 0,
	lcr_BREAK_SIGNAL_DISABLE_MASK = 64,
	lcr_BREAK_SIGNAL_ENABLE = 1,
	lcr_BREAK_SIGNAL_ENABLE_VALUE = 64,
	lcr_BREAK_SIGNAL_ENABLE_MASK = 64,
	lcr_DLAB_BUFFERS = 0,
	lcr_DLAB_BUFFERS_VALUE = 0,
	lcr_DLAB_BUFFERS_MASK = 128,
	lcr_DLAB_CLOCK = 1,
	lcr_DLAB_CLOCK_VALUE = 128,
	lcr_DLAB_CLOCK_MASK = 128
};
enum e_mcr 
{
	mcr_DTR_DTR = 1,
	mcr_DTR_DTR_VALUE = 1,
	mcr_DTR_DTR_MASK = 1,
	mcr_RTS_RTS = 1,
	mcr_RTS_RTS_VALUE = 2,
	mcr_RTS_RTS_MASK = 2,
	mcr_AO2_AO2 = 1,
	mcr_AO2_AO2_VALUE = 4,
	mcr_AO2_AO2_MASK = 4
};
enum e_lsr 
{
	lsr_DATA_AVAILABLE_FALSE = 0,
	lsr_DATA_AVAILABLE_FALSE_VALUE = 0,
	lsr_DATA_AVAILABLE_FALSE_MASK = 1,
	lsr_DATA_AVAILABLE_TRUE = 1,
	lsr_DATA_AVAILABLE_TRUE_VALUE = 1,
	lsr_DATA_AVAILABLE_TRUE_MASK = 1,
	lsr_OVERRUN_GOOD = 0,
	lsr_OVERRUN_GOOD_VALUE = 0,
	lsr_OVERRUN_GOOD_MASK = 2,
	lsr_OVERRUN_ERROR = 1,
	lsr_OVERRUN_ERROR_VALUE = 2,
	lsr_OVERRUN_ERROR_MASK = 2,
	lsr_PARITY_GOOD = 0,
	lsr_PARITY_GOOD_VALUE = 0,
	lsr_PARITY_GOOD_MASK = 4,
	lsr_PARITY_ERROR = 1,
	lsr_PARITY_ERROR_VALUE = 4,
	lsr_PARITY_ERROR_MASK = 4,
	lsr_FRAMING_GOOD = 0,
	lsr_FRAMING_GOOD_VALUE = 0,
	lsr_FRAMING_GOOD_MASK = 8,
	lsr_FRAMING_ERROR = 1,
	lsr_FRAMING_ERROR_VALUE = 8,
	lsr_FRAMING_ERROR_MASK = 8,
	lsr_BREAK_SIGNAL_TRUE = 0,
	lsr_BREAK_SIGNAL_TRUE_VALUE = 0,
	lsr_BREAK_SIGNAL_TRUE_MASK = 16,
	lsr_BREAK_SIGNAL_FALSE = 1,
	lsr_BREAK_SIGNAL_FALSE_VALUE = 16,
	lsr_BREAK_SIGNAL_FALSE_MASK = 16,
	lsr_THR_STATE_TRANSMITTING = 0,
	lsr_THR_STATE_TRANSMITTING_VALUE = 0,
	lsr_THR_STATE_TRANSMITTING_MASK = 32,
	lsr_THR_STATE_EMPTY = 1,
	lsr_THR_STATE_EMPTY_VALUE = 32,
	lsr_THR_STATE_EMPTY_MASK = 32,
	lsr_THR_AND_LINE_TRANSMITTING = 0,
	lsr_THR_AND_LINE_TRANSMITTING_VALUE = 0,
	lsr_THR_AND_LINE_TRANSMITTING_MASK = 64,
	lsr_THR_AND_LINE_EMPTY_IDLE = 1,
	lsr_THR_AND_LINE_EMPTY_IDLE_VALUE = 64,
	lsr_THR_AND_LINE_EMPTY_IDLE_MASK = 64,
	lsr_DATA_FIFO_GOOD = 0,
	lsr_DATA_FIFO_GOOD_VALUE = 0,
	lsr_DATA_FIFO_GOOD_MASK = 128,
	lsr_DATA_FIFO_ERROR = 1,
	lsr_DATA_FIFO_ERROR_VALUE = 128,
	lsr_DATA_FIFO_ERROR_MASK = 128
};
typedef struct s_rcv_buff_register 
{
	signed int __UNDEF_0_  : 1;
	signed int __UNDEF_1_  : 1;
	signed int __UNDEF_2_  : 1;
	signed int __UNDEF_3_  : 1;
	signed int __UNDEF_4_  : 1;
	signed int __UNDEF_5_  : 1;
	signed int __UNDEF_6_  : 1;
	signed int __UNDEF_7_  : 1;
} t_rcv_buff_register;
typedef struct s_snd_buff_register 
{
	signed int __UNDEF_0_  : 1;
	signed int __UNDEF_1_  : 1;
	signed int __UNDEF_2_  : 1;
	signed int __UNDEF_3_  : 1;
	signed int __UNDEF_4_  : 1;
	signed int __UNDEF_5_  : 1;
	signed int __UNDEF_6_  : 1;
	signed int __UNDEF_7_  : 1;
} t_snd_buff_register;
typedef struct s_ier_register 
{
	signed int DATA_AVAILABLE  : 1;
	signed int TRANSMITTER_EMPTY  : 1;
	signed int LINE_STATUS_CHANGE  : 1;
	signed int MODEM_STATUS_CHANGE  : 1;
	signed int SLEEP_MODE  : 1;
	signed int LOW_POWER_MODE  : 1;
	signed int __UNDEF_6_  : 1;
	signed int __UNDEF_7_  : 1;
} t_ier_register;
typedef struct s_lcr_register 
{
	signed int WORD_LENGH  : 2;
	signed int STOP_BITS  : 1;
	signed int PARITY_TYPE  : 3;
	signed int BREAK_SIGNAL  : 1;
	signed int DLAB  : 1;
} t_lcr_register;
typedef struct s_mcr_register 
{
	signed int DTR  : 1;
	signed int RTS  : 1;
	signed int __UNDEF_2_  : 1;
	signed int AO2  : 1;
	signed int __UNDEF_4_  : 1;
	signed int __UNDEF_5_  : 1;
	signed int __UNDEF_6_  : 1;
	signed int __UNDEF_7_  : 1;
} t_mcr_register;
typedef struct s_dll_register 
{
	signed int __UNDEF_0_  : 1;
	signed int __UNDEF_1_  : 1;
	signed int __UNDEF_2_  : 1;
	signed int __UNDEF_3_  : 1;
	signed int __UNDEF_4_  : 1;
	signed int __UNDEF_5_  : 1;
	signed int __UNDEF_6_  : 1;
	signed int __UNDEF_7_  : 1;
} t_dll_register;
typedef struct s_dlm_register 
{
	signed int __UNDEF_0_  : 1;
	signed int __UNDEF_1_  : 1;
	signed int __UNDEF_2_  : 1;
	signed int __UNDEF_3_  : 1;
	signed int __UNDEF_4_  : 1;
	signed int __UNDEF_5_  : 1;
	signed int __UNDEF_6_  : 1;
	signed int __UNDEF_7_  : 1;
} t_dlm_register;
typedef struct s_lsr_register 
{
	signed int DATA_AVAILABLE  : 1;
	signed int OVERRUN  : 1;
	signed int PARITY  : 1;
	signed int FRAMING  : 1;
	signed int BREAK_SIGNAL  : 1;
	signed int THR_STATE  : 1;
	signed int THR_AND_LINE  : 1;
	signed int DATA_FIFO  : 1;
} t_lsr_register;
typedef struct s_rcv_buff 
{
	t_rcv_buff_register rcv_buff_register;
	t_rcv_buff_register (*rcv_buff_inb) (t_iobase );
	void (*rcv_buff_outb) (t_iobase , t_rcv_buff_register );
	signed char (*rcv_buff_inb_value) (t_iobase );
	void (*rcv_buff_outb_value) (t_iobase , signed char );
} t_rcv_buff;
typedef struct s_snd_buff 
{
	t_snd_buff_register snd_buff_register;
	t_snd_buff_register (*snd_buff_inb) (t_iobase );
	void (*snd_buff_outb) (t_iobase , t_snd_buff_register );
	signed char (*snd_buff_inb_value) (t_iobase );
	void (*snd_buff_outb_value) (t_iobase , signed char );
} t_snd_buff;
typedef struct s_ier 
{
	t_ier_register ier_register;
	t_ier_register (*ier_inb) (t_iobase );
	void (*ier_outb) (t_iobase , t_ier_register );
	signed char (*ier_inb_value) (t_iobase );
	void (*ier_outb_value) (t_iobase , signed char );
} t_ier;
typedef struct s_lcr 
{
	t_lcr_register lcr_register;
	t_lcr_register (*lcr_inb) (t_iobase );
	void (*lcr_outb) (t_iobase , t_lcr_register );
	signed char (*lcr_inb_value) (t_iobase );
	void (*lcr_outb_value) (t_iobase , signed char );
} t_lcr;
typedef struct s_mcr 
{
	t_mcr_register mcr_register;
	t_mcr_register (*mcr_inb) (t_iobase );
	void (*mcr_outb) (t_iobase , t_mcr_register );
	signed char (*mcr_inb_value) (t_iobase );
	void (*mcr_outb_value) (t_iobase , signed char );
} t_mcr;
typedef struct s_dll 
{
	t_dll_register dll_register;
	t_dll_register (*dll_inb) (t_iobase );
	void (*dll_outb) (t_iobase , t_dll_register );
	signed char (*dll_inb_value) (t_iobase );
	void (*dll_outb_value) (t_iobase , signed char );
} t_dll;
typedef struct s_dlm 
{
	t_dlm_register dlm_register;
	t_dlm_register (*dlm_inb) (t_iobase );
	void (*dlm_outb) (t_iobase , t_dlm_register );
	signed char (*dlm_inb_value) (t_iobase );
	void (*dlm_outb_value) (t_iobase , signed char );
} t_dlm;
typedef struct s_lsr 
{
	t_lsr_register lsr_register;
	t_lsr_register (*lsr_inb) (t_iobase );
	void (*lsr_outb) (t_iobase , t_lsr_register );
	signed char (*lsr_inb_value) (t_iobase );
	void (*lsr_outb_value) (t_iobase , signed char );
} t_lsr;
t_rcv_buff_register rcv_buff_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_rcv_buff_register	*)(  void	*)&c;
}

signed char rcv_buff_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void rcv_buff_outb (t_iobase iobase, t_rcv_buff_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void rcv_buff_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_rcv_buff 	* new_rcv_buff (void )
{
	t_rcv_buff 	* rcv_buff;
	rcv_buff = MmAllocateNonCachedMemory(1 * sizeof (*rcv_buff));
	if (rcv_buff)
	{
		memset(&rcv_buff->rcv_buff_register, 0, sizeof (rcv_buff->rcv_buff_register));
		rcv_buff->rcv_buff_inb = rcv_buff_inb;
		rcv_buff->rcv_buff_outb = rcv_buff_outb;
		rcv_buff->rcv_buff_inb_value = rcv_buff_inb_value;
		rcv_buff->rcv_buff_outb_value = rcv_buff_outb_value;
	}

	return (rcv_buff);
}

t_snd_buff_register snd_buff_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_snd_buff_register	*)(  void	*)&c;
}

signed char snd_buff_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void snd_buff_outb (t_iobase iobase, t_snd_buff_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void snd_buff_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_snd_buff 	* new_snd_buff (void )
{
	t_snd_buff 	* snd_buff;
	snd_buff = MmAllocateNonCachedMemory(1 * sizeof (*snd_buff));
	if (snd_buff)
	{
		memset(&snd_buff->snd_buff_register, 0, sizeof (snd_buff->snd_buff_register));
		snd_buff->snd_buff_inb = snd_buff_inb;
		snd_buff->snd_buff_outb = snd_buff_outb;
		snd_buff->snd_buff_inb_value = snd_buff_inb_value;
		snd_buff->snd_buff_outb_value = snd_buff_outb_value;
	}

	return (snd_buff);
}

t_ier_register ier_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_ier_register	*)(  void	*)&c;
}

signed char ier_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void ier_outb (t_iobase iobase, t_ier_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void ier_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_ier 	* new_ier (void )
{
	t_ier 	* ier;
	ier = MmAllocateNonCachedMemory(1 * sizeof (*ier));
	if (ier)
	{
		memset(&ier->ier_register, 0, sizeof (ier->ier_register));
		ier->ier_inb = ier_inb;
		ier->ier_outb = ier_outb;
		ier->ier_inb_value = ier_inb_value;
		ier->ier_outb_value = ier_outb_value;
	}

	return (ier);
}

t_lcr_register lcr_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_lcr_register	*)(  void	*)&c;
}

signed char lcr_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void lcr_outb (t_iobase iobase, t_lcr_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void lcr_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_lcr 	* new_lcr (void )
{
	t_lcr 	* lcr;
	lcr = MmAllocateNonCachedMemory(1 * sizeof (*lcr));
	if (lcr)
	{
		memset(&lcr->lcr_register, 0, sizeof (lcr->lcr_register));
		lcr->lcr_inb = lcr_inb;
		lcr->lcr_outb = lcr_outb;
		lcr->lcr_inb_value = lcr_inb_value;
		lcr->lcr_outb_value = lcr_outb_value;
	}

	return (lcr);
}

t_mcr_register mcr_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_mcr_register	*)(  void	*)&c;
}

signed char mcr_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void mcr_outb (t_iobase iobase, t_mcr_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void mcr_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_mcr 	* new_mcr (void )
{
	t_mcr 	* mcr;
	mcr = MmAllocateNonCachedMemory(1 * sizeof (*mcr));
	if (mcr)
	{
		memset(&mcr->mcr_register, 0, sizeof (mcr->mcr_register));
		mcr->mcr_inb = mcr_inb;
		mcr->mcr_outb = mcr_outb;
		mcr->mcr_inb_value = mcr_inb_value;
		mcr->mcr_outb_value = mcr_outb_value;
	}

	return (mcr);
}

t_dll_register dll_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_dll_register	*)(  void	*)&c;
}

signed char dll_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void dll_outb (t_iobase iobase, t_dll_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void dll_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_dll 	* new_dll (void )
{
	t_dll 	* dll;
	dll = MmAllocateNonCachedMemory(1 * sizeof (*dll));
	if (dll)
	{
		memset(&dll->dll_register, 0, sizeof (dll->dll_register));
		dll->dll_inb = dll_inb;
		dll->dll_outb = dll_outb;
		dll->dll_inb_value = dll_inb_value;
		dll->dll_outb_value = dll_outb_value;
	}

	return (dll);
}

t_dlm_register dlm_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_dlm_register	*)(  void	*)&c;
}

signed char dlm_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void dlm_outb (t_iobase iobase, t_dlm_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void dlm_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_dlm 	* new_dlm (void )
{
	t_dlm 	* dlm;
	dlm = MmAllocateNonCachedMemory(1 * sizeof (*dlm));
	if (dlm)
	{
		memset(&dlm->dlm_register, 0, sizeof (dlm->dlm_register));
		dlm->dlm_inb = dlm_inb;
		dlm->dlm_outb = dlm_outb;
		dlm->dlm_inb_value = dlm_inb_value;
		dlm->dlm_outb_value = dlm_outb_value;
	}

	return (dlm);
}

t_lsr_register lsr_inb (t_iobase iobase)
{
	signed char c  = READ_PORT_UCHAR(iobase);
	return *(  t_lsr_register	*)(  void	*)&c;
}

signed char lsr_inb_value (t_iobase iobase)
{
	return READ_PORT_UCHAR(iobase);
}

void lsr_outb (t_iobase iobase, t_lsr_register value)
{
	WRITE_PORT_UCHAR(iobase, *(signed  char	*)(  void	*)&value);
}

void lsr_outb_value (t_iobase iobase, signed char value)
{
	WRITE_PORT_UCHAR(iobase, value);
}

t_lsr 	* new_lsr (void )
{
	t_lsr 	* lsr;
	lsr = MmAllocateNonCachedMemory(1 * sizeof (*lsr));
	if (lsr)
	{
		memset(&lsr->lsr_register, 0, sizeof (lsr->lsr_register));
		lsr->lsr_inb = lsr_inb;
		lsr->lsr_outb = lsr_outb;
		lsr->lsr_inb_value = lsr_inb_value;
		lsr->lsr_outb_value = lsr_outb_value;
	}

	return (lsr);
}

void my_rs232_set (WORD value)
{
	t_lcr 	* lcr_register_object;
	t_dll 	* dll_register_object;
	t_dlm 	* dlm_register_object;
	lcr_register_object = new_lcr();
	dll_register_object = new_dll();
	dlm_register_object = new_dlm();
	lcr_register_object->lcr_register = lcr_register_object->lcr_inb(0x03f8 + 3);
	lcr_register_object->lcr_register.DLAB = 1;
	lcr_register_object->lcr_outb(0x03f8 + 3, lcr_register_object->lcr_register);
	dll_register_object->dll_register = dll_register_object->dll_inb(0x03f8 + 0);
	dll_register_object->dll_outb_value(0x03f8 + 0, (value >> 0) & (511));
	dlm_register_object->dlm_register = dlm_register_object->dlm_inb(0x03f8 + 1);
	dlm_register_object->dlm_outb_value(0x03f8 + 1, (value >> 8) & (511));
	lcr_register_object->lcr_register = lcr_register_object->lcr_inb(0x03f8 + 3);
	lcr_register_object->lcr_register.DLAB = 0;
	lcr_register_object->lcr_outb(0x03f8 + 3, lcr_register_object->lcr_register);
	exit :

	return;
}

WORD my_rs232_get ()
{
	WORD value;
	WORD temp;
	t_lcr 	* lcr_register_object;
	t_dlm 	* dlm_register_object;
	t_dll 	* dll_register_object;
	lcr_register_object = new_lcr();
	dlm_register_object = new_dlm();
	dll_register_object = new_dll();
	lcr_register_object->lcr_register = lcr_register_object->lcr_inb(0x03f8 + 3);
	lcr_register_object->lcr_register.DLAB = 1;
	lcr_register_object->lcr_outb(0x03f8 + 3, lcr_register_object->lcr_register);
	temp = 0x0;
	temp = dlm_register_object->dlm_inb_value(0x03f8 + 1);
	value = value & ~(511 << 0);
	value = value | ((temp << 0) & (511 << 0));
	temp = 0x0;
	temp = dll_register_object->dll_inb_value(0x03f8 + 0);
	value = value & ~(511 << 8);
	value = value | ((temp << 8) & (511 << 8));
	lcr_register_object->lcr_register = lcr_register_object->lcr_inb(0x03f8 + 3);
	lcr_register_object->lcr_register.DLAB = 0;
	lcr_register_object->lcr_outb(0x03f8 + 3, lcr_register_object->lcr_register);
	exit :

	return value;
}

void my_rs232IoRead (WDFQUEUE Queue, WDFREQUEST Request, size_t size)
{
	WDFMEMORY memory;
	NTSTATUS status;
	PUCHAR output;
	signed int current_size  = 0;
	t_lsr 	* lsr_register_object;
	t_rcv_buff 	* rcv_buff_register_object;
	status = WdfRequestRetrieveOutputMemory(Request, &memory);
	if (NT_SUCCESS(status))
	{
		output = MmAllocateNonCachedMemory(size);
		memset(output, 0, size);
		rcv_buff_register_object = new_rcv_buff();
		KdPrint(("%s" , "my_rs232 [read]\n"));
		while (current_size < size)
		{
			lsr_register_object = new_lsr();
			while ((lsr_register_object->lsr_inb_value(0x03f8 + 5) & lsr_DATA_AVAILABLE_TRUE_MASK) != lsr_DATA_AVAILABLE_TRUE_VALUE)
			;

			output[current_size] = rcv_buff_register_object->rcv_buff_inb_value(0x03f8 + 0);
			current_size++;
		}

		status = WdfMemoryCopyFromBuffer(memory, 0, output, current_size);
		WdfRequestCompleteWithInformation(Request, status, current_size);
		MmFreeNonCachedMemory(output, size);
	}
	else
	{
		WdfRequestComplete(Request, status);
	}

}

void my_rs232IoWrite (WDFQUEUE Queue, WDFREQUEST Request, size_t size)
{
	WDFMEMORY memory;
	NTSTATUS status;
	PUCHAR input;
	signed int current_size  = 0;
	t_lsr 	* lsr_register_object;
	t_snd_buff 	* snd_buff_register_object;
	KdPrint(("-->EvtIoWrite\n"));
	status = WdfRequestRetrieveInputMemory(Request, &memory);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("EvtIoWrite Could not get request memory buffer 0x%x\n" , status));
		WdfRequestComplete(Request, status);
		KdPrint(("<-- EvtDeviceIoWrite\n"));
		return;
	}

	input = WdfMemoryGetBuffer(memory, &size);
	KdPrint(("Sending a buffer of %d bytes\n" , size));
	lsr_register_object = new_lsr();
	snd_buff_register_object = new_snd_buff();
	KdPrint(("%s" , "my_rs232 [write]\n"));
	while ((lsr_register_object->lsr_inb_value(0x03f8 + 5) & lsr_THR_AND_LINE_EMPTY_IDLE_MASK) != lsr_THR_AND_LINE_EMPTY_IDLE_VALUE)
	;

	while (current_size < size)
	{
		lsr_register_object = new_lsr();
		while ((lsr_register_object->lsr_inb_value(0x03f8 + 5) & lsr_THR_STATE_EMPTY_MASK) != lsr_THR_STATE_EMPTY_VALUE)
		;

		snd_buff_register_object->snd_buff_outb_value(0x03f8 + 0, input[current_size]);
		current_size++;
	}

	WdfRequestCompleteWithInformation(Request, status, current_size);
	KdPrint(("<--EvtIoWrite\n"));
}

NTSTATUS my_rs232_on_plug (WDFDEVICE Device, WDFCMRESLIST Resources, WDFCMRESLIST ResourcesTranslated)
{
	t_lcr 	* lcr_register_object;
	t_mcr 	* mcr_register_object;
	lcr_register_object = new_lcr();
	mcr_register_object = new_mcr();
	KdPrint(("%s" , "my_rs232 [on_plug]\n"));
	my_rs232_set(0x0c);
	lcr_register_object->lcr_register = lcr_register_object->lcr_inb(0x03f8 + 3);
	lcr_register_object->lcr_register.WORD_LENGH = lcr_WORD_LENGH__8BITS;
	lcr_register_object->lcr_outb(0x03f8 + 3, lcr_register_object->lcr_register);
	lcr_register_object->lcr_register = lcr_register_object->lcr_inb(0x03f8 + 3);
	lcr_register_object->lcr_register.STOP_BITS = lcr_STOP_BITS__1STOP_BITS;
	lcr_register_object->lcr_outb(0x03f8 + 3, lcr_register_object->lcr_register);
	lcr_register_object->lcr_register = lcr_register_object->lcr_inb(0x03f8 + 3);
	lcr_register_object->lcr_register.PARITY_TYPE = lcr_PARITY_TYPE_NONE;
	lcr_register_object->lcr_outb(0x03f8 + 3, lcr_register_object->lcr_register);
	mcr_register_object->mcr_register = mcr_register_object->mcr_inb(0x03f8 + 4);
	mcr_register_object->mcr_register.RTS = 1;
	mcr_register_object->mcr_outb(0x03f8 + 4, mcr_register_object->mcr_register);
	mcr_register_object->mcr_register = mcr_register_object->mcr_inb(0x03f8 + 4);
	mcr_register_object->mcr_register.DTR = 1;
	mcr_register_object->mcr_outb(0x03f8 + 4, mcr_register_object->mcr_register);
	exit :

	return STATUS_SUCCESS;
}

NTSTATUS EvtDeviceAdd (WDFDRIVER Driver, PWDFDEVICE_INIT DeviceInit)
{
	NTSTATUS status;
	WDFDEVICE device;
	WDFQUEUE defaultqueue;
	WDF_IO_QUEUE_CONFIG defaultconfig;
	WDF_PNPPOWER_EVENT_CALLBACKS pnpPowerCallbacks;
	WDF_PNPPOWER_EVENT_CALLBACKS_INIT(&pnpPowerCallbacks);
	pnpPowerCallbacks.EvtDevicePrepareHardware = my_rs232_on_plug;
	WdfDeviceInitSetPnpPowerEventCallbacks(DeviceInit, &pnpPowerCallbacks);
	status = WdfDeviceCreate(&DeviceInit, WDF_NO_OBJECT_ATTRIBUTES, &device);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("my_rs232WdfDeviceCreate failed\n"));
		return (status);
	}

	WDF_IO_QUEUE_CONFIG_INIT_DEFAULT_QUEUE(&defaultconfig, WdfIoQueueDispatchParallel);
	defaultconfig.EvtIoRead = my_rs232IoRead;
	defaultconfig.EvtIoWrite = my_rs232IoWrite;
	status = WdfIoQueueCreate(device, &defaultconfig, WDF_NO_OBJECT_ATTRIBUTES, &defaultqueue);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("my_rs232WdfIoQueueCreate failed\n"));
	}

	status = WdfDeviceCreateDeviceInterface(device, &GUID_DEVINTERFACE_RATHAXES_SERIAL, NULL);
	return (status);
}

NTSTATUS DriverEntry (PDRIVER_OBJECT DriverObject, PUNICODE_STRING RegistryPath)
{
	NTSTATUS status  = STATUS_SUCCESS;
	WDF_DRIVER_CONFIG config;
	KdPrint(("my_rs232-->DriverEntry\n"));
	WDF_DRIVER_CONFIG_INIT(&config, EvtDeviceAdd);
	status = WdfDriverCreate(DriverObject, RegistryPath, WDF_NO_OBJECT_ATTRIBUTES, &config, WDF_NO_HANDLE);
	if (!NT_SUCCESS(status))
	{
		KdPrint(("my_rs232 - Error: WdfDriverCreate failed\n"));
	}

	KdPrint(("my_rs232<--DriverEntry\n"));
	return (status);
}

