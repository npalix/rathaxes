#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>

#include "e1000.h"

#define INTEL_ID	0x8086
#define CARD_ID	0x100E

/****************************************************************************/
/*******************************ACTUAL DRIVER IMPL***************************/
/****************************************************************************/
static int		e1000_open(struct net_device *dev);
static int		e1000_close(struct net_device *dev);
static void		e1000_set_mac_address(void);
static netdev_tx_t 	e1000_start_xmit(struct sk_buff *skb,
					 struct net_device *dev);
static void init_hw(struct net_device* dev);
static void uninit_hw(struct net_device* dev);
static void init_queue(struct net_device* dev);
static void uninit_queue(struct net_device* dev);

static struct net_device *card = NULL;
static unsigned char mac_address[MAX_ADDR_LEN];

static const struct net_device_ops e1000_ops = {
	.ndo_open = e1000_open,
	.ndo_stop = e1000_close,
	.ndo_start_xmit = e1000_start_xmit,

//  .ndo_get_stats                      = not_implemented_fct,
//  .ndo_set_multicast_list     = not_implemented_fct,
//  .ndo_tx_timeout                     = not_implemented_fct,
//  .ndo_validate_addr          = not_implemented_fct,
//  .ndo_change_mtu                     = not_implemented_fct,
//  .ndo_set_mac_address                = not_implemented_fct,
};

static int e1000_open(struct net_device *dev)
{
	t_e1000_data *data;

	printk(KERN_ERR "Open the e1000 card\n");
	e1000_set_mac_address();

	data = netdev_priv(dev);
	init_hw(dev);
	init_queue(dev);

	netif_start_queue(dev);
	return 0;
}

static int e1000_close(struct net_device *dev)
{
	printk(KERN_ERR "Close the e1000 card\n");
	netif_stop_queue(dev);
	uninit_queue(dev);
	uninit_hw(dev);
	return 0;
}

static netdev_tx_t e1000_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	printk(KERN_WARNING "e1000_start_xmit called\n");
	return NETDEV_TX_OK;
}

static void init_hw(struct net_device* dev)
{
	t_e1000_data *data;
	int i;

	data = netdev_priv(dev);

	/* Take a look to the Intel Manual Developper
	 * section: 14.3
	 */

	/* Activate automatic speed detection
	 * it requires the ASDE and the SLU bit
	 * set in the control register
	 */

	e1000_set_register_preserve(data, CTRL_REG,
			CTRL_ASDE| CTRL_SLU);

	/*
	 * Activate automatic negociation
	 */

	e1000_unset_register_preserve(data, CTRL_REG,
			CTRL_RST);

	e1000_unset_register_preserve(data, CTRL_REG,
			CTRL_PHYRST);

	/*
	 * Desactivate flow control
	 */

	e1000_unset_register(data, FCAL_REG,  0);
	e1000_unset_register(data, FCAH_REG,  0);
	e1000_unset_register(data, FCT_REG,   0);
	e1000_unset_register(data, FCTTV_REG, 0);

	/*
	 * Do no support 802.1q vlan
	 */
	e1000_unset_register_preserve(data, CTRL_REG,
			CTRL_VMDE);

	/*
	 * Initialize the MTA table (128 bytes)
	 */

	for (i = 0; i < 128; ++i)
		e1000_set_register(data, MTA_REG + i, 0);

	/*
	 * "The Statistics registers are not hardware initialized. Their default value is unknown. 
	 * Software should read the contents of all registers in order to clear them prior to enabling
	 * the receive and transmit channels."
	 */

	/*
	 * Some addresses are read like the 4028h but it is not a stat
	 * register.
	 */
	for (i = 0; i < NB_STAT_REG; ++i)
		e1000_read_register(data, CRCERRS_REG + (i * 4));


}

static void uninit_hw(struct net_device* dev)
{
}

static void init_queue(struct net_device* dev)
{
	t_e1000_data* data;
	uint32_t phys;
	int i;

	data = netdev_priv(dev);

	/*
	 * use NB_RCV_DESC with BUF_SIZE_BY_DESC size
	 * for each
	 */

	data->recv.descriptors = kmalloc(sizeof(*data->recv.descriptors) * NB_RCV_DESC,
			GFP_KERNEL);

	if (((uint32_t)data->recv.descriptors) % 16)
	{
		printk(KERN_ERR "Address non aligned, reboot :(");
		return ;
	}

	data->recv.buff = (uint32_t) kmalloc(BUF_SIZE_BY_DESC * NB_RCV_DESC,
			GFP_KERNEL);

	memset(data->recv.descriptors, 0, sizeof(*data->recv.descriptors) * NB_RCV_DESC);

	phys = virt_to_phys((volatile void*)data->recv.buff);
	/* init the buffer address for each descriptor */
	for (i = 0; i < NB_RCV_DESC; ++i)
		data->recv.descriptors[i].address_l = phys + (i * BUF_SIZE_BY_DESC);

	phys = virt_to_phys(data->recv.descriptors);

	/* Initialize the ring buffer (address, len, head, tail) */
	e1000_set_register(data, RDBAL_REG, phys);
	e1000_set_register(data, RDBAH_REG, 0);
	e1000_set_register(data, RDLEN_REG, sizeof(*data->recv.descriptors) * NB_RCV_DESC);
	e1000_set_register(data, RDH_REG, 0);
	e1000_set_register(data, RDT_REG, NB_RCV_DESC - 1);

	/* Set the size of the buffer in control register */
	e1000_unset_register_preserve(data, RCTL_REG, RCTL_BSEX);
	e1000_unset_register_preserve(data, RCTL_REG, RCTL_BSIZE_CLR);
	e1000_set_register_preserve(data,   RCTL_REG, RCTL_BSIZE_2048);

	/* enable receiving */
	e1000_set_register_preserve(data, RCTL_REG, RCTL_ENABLE);


	/*** TRANSMISSION SETUP ***/

	data->send.descriptors = kmalloc(sizeof(*data->send.descriptors) * NB_SND_DESC,
			GFP_KERNEL);

	if (((uint32_t)data->send.descriptors) % 16)
	{
		printk(KERN_ERR "Address non aligned, reboot :(");
		return ;
	}

	data->send.buff = (uint32_t) kmalloc(BUF_SIZE_BY_DESC * NB_SND_DESC,
			GFP_KERNEL);

	memset(data->send.descriptors, 0, sizeof(*data->send.descriptors) * NB_SND_DESC);

	phys = virt_to_phys((volatile void*)data->send.buff);
	/* init the buffer address for each descriptor */
	for (i = 0; i < NB_SND_DESC; ++i)
		data->send.descriptors[i].address_l = phys + (i * BUF_SIZE_BY_DESC);

	phys = virt_to_phys(data->send.descriptors);
	e1000_set_register(data, TDBAL_REG, phys);
	e1000_set_register(data, TDBAH_REG, 0);
	e1000_set_register(data, TDLEN_REG, sizeof(*data->send.descriptors) * NB_SND_DESC);
	e1000_set_register(data, TDH_REG, 0);
	e1000_set_register(data, TDT_REG, NB_SND_DESC - 1);

	/* enable transmitting */
	e1000_set_register_preserve(data, TCTL_REG, TCTL_ENABLE | TCTL_PAD_PACK);
}

static void uninit_queue(struct net_device* dev)
{
	t_e1000_data* data;

	data = netdev_priv(dev);
	e1000_unset_register_preserve(data, RCTL_REG, RCTL_ENABLE);
	kfree((void*)data->recv.buff);
	kfree(data->recv.descriptors);
}

static int register_hw(struct pci_dev *dev)
{
	if (dev == 0) {
		printk(KERN_ERR "Error  dev == 0\n");
		return -EFAULT;
	}

	card = alloc_etherdev(sizeof(t_e1000_data));

	if (!card)
		return -ENOMEM;

	SET_NETDEV_DEV(card, &dev->dev);
	card->netdev_ops = &e1000_ops;

	if (register_netdev(card)) {
		printk(KERN_ERR "Error when registring e1000 driver: %s\n",
		       card->name);
		return -EFAULT;
	}

	printk(KERN_WARNING "Ethernet card has name: %s\n", card->name);

	return 0;
}

static void e1000_set_mac_address(void)
{
	t_e1000_data *data;
	int i, tmp;

	data = netdev_priv(card);
	for (i = 0; i < 3; ++i) {
		tmp = e1000_read_eeprom(data, i);
		mac_address[(i * 2)] = tmp & 0xFF;
		mac_address[(i * 2) + 1] = (tmp & 0xFF00) >> 8;
	}

	card->dev_addr = mac_address;
}

static void unload_hw(void)
{
	if (card) {
		unregister_netdev(card);
		free_netdev(card);
	}
}

/****************************************************************************/
/****************************PCI SPECIFIC************************************/
/****************************************************************************/

static struct pci_device_id supported_cards[] = {
	{PCI_DEVICE(INTEL_ID, CARD_ID),},
	{0,}
};

MODULE_DEVICE_TABLE(pci, supported_cards);

static int probe(struct pci_dev *dev, const struct pci_device_id *id)
{
	u32 addr;
	u8 revision;
	u8 interrupt;
	int err;
	t_e1000_data *data;

	printk(KERN_WARNING "PROB function called\n");

	pci_read_config_byte(dev, PCI_REVISION_ID, &revision);
	printk(KERN_WARNING "Revision: %x\n", (u32) revision);

	pci_read_config_byte(dev, PCI_INTERRUPT_LINE, &interrupt);
	printk(KERN_WARNING "Interrupt number used: %i\n", (u32) interrupt);

	pci_read_config_dword(dev, PCI_ROM_ADDRESS, &addr);
	printk(KERN_WARNING "Rom address: %x\n", addr);

	pci_read_config_dword(dev, PCI_BASE_ADDRESS_0, &addr);
	printk(KERN_WARNING "BAR0: %x\n", addr);

	if ((err = register_hw(dev))) {
		printk(KERN_ERR "An error occured when registering e1000 driver\n");
		return err;
	}

	if ((err = pci_enable_device(dev))) {
		return err;
	}

	if ((err = pci_request_region(dev, 0, "e1000_BAR_0"))) {
		return err;
	}

	data = netdev_priv(card);
	data->bar = (u32) pci_ioremap_bar(dev, 0);

	return 0;
}

void remove(struct pci_dev *dev)
{
	printk(KERN_WARNING "remove called\n");
	unload_hw();
	pci_release_region(dev, 0);
}

static struct pci_driver e1000_driver = {
	.name = "e1000_daedric_driver",
	.id_table = supported_cards,
	.probe = probe,
	.remove = remove,
};

static int __init init(void)
{
	int err;
	printk(KERN_WARNING "HELLO WORLD\n");

	if ((err = pci_register_driver(&e1000_driver))) {
		printk(KERN_ERR "An error occured when registering e1000 driver\n");
		return err;
	}

	return 0;

}

static void __exit cleanup(void)
{
	printk(KERN_WARNING "BYE WORLD\n");
	pci_unregister_driver(&e1000_driver);
}

module_init(init);
module_exit(cleanup);
MODULE_LICENSE("GPL");
