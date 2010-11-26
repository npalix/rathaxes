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
int e1000_read_eeprom(t_e1000_data* data, int addr)
{
  int	res;

  ((uint32_t *)(data->bar + EEPROM_READ))[0]= (addr << 8) | 1;
  while(!((res = ((uint32_t*)(data->bar + EEPROM_READ))[0]) & (1 << 4)));
  return res >> 16;
}


#endif
