#ifndef __E1000_REG_PRIMITIVE__
# define __E1000_REG_PRIMITIVE__

static inline
uint32_t* e1000_get_register(t_e1000_data* data, enum e_register reg)
{
	return (uint32_t*)(data->bar + reg);
}

static inline
uint32_t e1000_read_register(t_e1000_data* data, enum e_register reg)
{
	return readl(e1000_get_register(data, reg));
}

static inline
void e1000_set_register_preserve(t_e1000_data* data, enum e_register reg, uint32_t value)
{
	uint32_t old;

	old = e1000_read_register(data, reg);
	writel(old | value, e1000_get_register(data, reg));
}

static inline
void e1000_set_register(t_e1000_data* data, enum e_register reg, uint32_t value)
{
	writel(value, e1000_get_register(data, reg));
}

static inline
void e1000_unset_register_preserve(t_e1000_data* data, enum e_register reg, uint32_t value)
{
	uint32_t old;

	old = e1000_read_register(data, reg);
	writel(old & ~value, e1000_get_register(data, reg));
}

#endif
