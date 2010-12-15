#ifndef __E1000_H__
# define __E1000_H__

/* view the structure section: 3.2.3 */
struct s_receive_descriptor
{
	uint32_t	address_l;
	uint32_t	address_h;
	uint16_t	length;
	uint16_t	checksum;
	uint8_t		status;
	uint8_t		errors;
	uint16_t	special;
};

struct s_transmit_descriptor
{
	uint32_t	address_l;
	uint32_t	address_h;
	uint16_t	length;
	uint16_t	checksum_offset;
	uint8_t		command;
	uint8_t		status;
//	uint8_t		reserved;
	uint8_t		checksum_start;
	uint16_t	special;
};

typedef struct s_receive_descriptor t_recv_desc;
typedef struct s_transmit_descriptor t_send_desc;

# define NB_RCV_DESC		256
# define NB_SND_DESC		256
# define BUF_SIZE_BY_DESC	2048
struct s_e1000_data
{
  struct pci_dev* pci;
  struct net_device* netdev;
  uint8_t*	bar;
  uint32_t	irq;
  struct tasklet_struct intr_tasklet;

  int waiting_intr;

  struct
  {
	 t_recv_desc*	descriptors;
	 dma_addr_t	dma_desc;
	 uint32_t	size_desc;

	 void*		buff;
	 dma_addr_t	dma_buff;
	 uint32_t	size_buff;
  } recv;

  struct
  {
	 t_send_desc*	descriptors;
	 dma_addr_t	dma_desc;
	 uint32_t	size_desc;

	 void*		buff;
	 dma_addr_t	dma_buff;
	 uint32_t	size_buff;
  } send;
};

typedef struct s_e1000_data t_e1000_data;

enum e_register
{
	CTRL_REG	= 0x00000,
	STAT_REG	= 0x00008,
	EEPROM_READ	= 0x00014,
	FCAL_REG	= 0x00028,
	FCAH_REG	= 0x0002c,
	FCT_REG		= 0x00030,
	FCTTV_REG	= 0x00170,
	MTA_REG		= 0x05200,
	CRCERRS_REG	= 0x04000,
# define NB_STAT_REG 64
	RDBAL_REG	= 0x02800,
	RDBAH_REG	= 0x02804,
	RDLEN_REG	= 0x02808,
	RDH_REG		= 0x02810,
	RDT_REG		= 0x02818,
	RCTL_REG	= 0x00100,
	TCTL_REG	= 0x00400,
	TDBAL_REG	= 0x03800,
	TDBAH_REG	= 0x03804,
	TDLEN_REG	= 0x03808,
	TDH_REG		= 0x03810,
	TDT_REG		= 0x03818,
	RAL_REG		= 0x05400,
	RAH_REG		= 0x05404,
	IMS_REG		= 0x000D0,
	ICR_REG		= 0x000C0,
};

enum e_register_value
{
	CTRL_ASDE	= (1 << 5),
	CTRL_LRST	= (1 << 3),
	CTRL_SLU	= (1 << 6),
	CTRL_ILOS	= (1 << 7),
	CTRL_RST	= (1 << 26),
	CTRL_VMDE	= (1 << 30),
	CTRL_PHYRST	= (1 << 31),
	RCTL_ENABLE	= (1 << 1),
	RCTL_BSIZE	= ((1 << 16) | (1 << 17)),
	RCTL_BSEX	= (1 << 25),
	RCTL_MPE	= (1 << 4),
	TCTL_ENABLE	= (1 << 1),
	TCTL_PAD_PACK	= (1 << 3),
	TCTL_CT		= (0xF << 4),
	RAH_AV		= (1 << 31),
	IMS_TXDW	= (1 << 0),
	IMS_TXQE	= (1 << 1),
	IMS_LSC		= (1 << 2),
	IMS_RX0		= (1 << 6),
	IMS_RXT0	= (1 << 7),
};

enum e_rx_status
{
	RX_EOP		= (1 << 1),
};

enum e_tx_command
{
	TX_EOP		= (1 << 0),
	TX_IFCS		= (1 << 1),
	TX_RS		= (1 << 3),
};

#include "e1000_reg_primitive.h"

static inline
int e1000_read_eeprom(t_e1000_data* data, int addr)
{
  int	res;

  e1000_set_register(data, EEPROM_READ, (addr << 8) | 1);
  while(!((res = e1000_read_register(data, EEPROM_READ)) & (1 << 4)));
  return res >> 16;
}

#endif
