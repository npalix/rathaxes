#ifndef __E1000_H__
# define __E1000_H__

struct s_e1000_data
{
  uint32_t  bar;
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
};

enum e_register_value
{
	CTRL_ASDE	= (1 << 5),
	CTRL_RST	= (1 << 3),
	CTRL_SLU	= (1 << 6),
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
