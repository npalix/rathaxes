#ifndef __E1000_H__
# define __E1000_H__

struct s_e1000_data
{
  uint32_t  bar;
};

typedef struct s_e1000_data t_e1000_data;

enum e_register
{
  EEPROM_READ = 0x14,
};

static inline
uint32_t* e1000_get_register(t_e1000_data* data, enum e_register reg)
{
	return (uint32_t*)(data->bar + reg);
}

static inline
uint32_t e1000_read_register(t_e1000_data* data, enum e_register reg)
{
	return *e1000_get_register(data, reg);
}

static inline
void e1000_set_register_preserve(t_e1000_data* data, enum e_register reg, uint32_t value)
{
	uint32_t old;

	old = e1000_read_register(data, reg);
	*e1000_get_register(data, reg) = old | value;
}

static inline
void e1000_set_register(t_e1000_data* data, enum e_register reg, uint32_t value)
{
	*e1000_get_register(data, reg) = value;
}

static inline
int e1000_read_eeprom(t_e1000_data* data, int addr)
{
  int	res;

  e1000_set_register(data, EEPROM_READ, (addr << 8) | 1);
  while(!((res = e1000_read_register(data, EEPROM_READ)) & (1 << 4)));
  return res >> 16;
}


#endif
