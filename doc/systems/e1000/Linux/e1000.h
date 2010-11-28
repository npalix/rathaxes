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

typedef struct s_receive_descriptor t_recv_desc;

# define NB_RCV_DESC		512
# define BUF_SIZE_BY_DESC	2048
struct s_e1000_data
{
  uint32_t	bar;

  struct
  {
	 t_recv_desc*	descriptors;
	 uint32_t	buff;
  } recv;
};

typedef struct s_e1000_data t_e1000_data;

enum e_register
{
	CTRL_REG	= 0x00000,
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
};

enum e_register_value
{
	CTRL_ENABLE	= (1 << 1),
	CTRL_ASDE	= (1 << 5),
	CTRL_RST	= (1 << 3),
	CTRL_SLU	= (1 << 6),
	CTRL_BSIZE_2048	= (0 << 16),
	CTRL_BSIZE_1024	= (1 << 16),
	CTRL_BSIZE_512	= (2 << 16),
	CTRL_BSIZE_256	= (3 << 16),
	CTRL_BSIZE_CLR	= (3 << 16),
	CTRL_BSEX	= (1 << 25),
	CTRL_VMDE	= (1 << 30),
	CTRL_PHYRST	= (1 << 31),
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
