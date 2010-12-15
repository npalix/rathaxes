#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>
#include <linux/interrupt.h>

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
static netdev_tx_t	e1000_start_xmit(struct sk_buff *skb, struct net_device *dev);
static int		init_interrupt(struct net_device* dev);
static void		uninit_interrupt(struct net_device* dev);
static int		init_hw(struct net_device* dev);
static void		uninit_hw(struct net_device* dev);
static int		init_queue(struct net_device* dev);
static void		uninit_queue(struct net_device* dev);
static void		e1000_read(struct net_device* dev);
irqreturn_t		handler(int irq, void* dev_id);
static void		intr_task(unsigned long pdata);


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

static void e1000_print_status(t_e1000_data* data)
{
	int status;

	status = e1000_read_register(data, STAT_REG);

	printk(KERN_ERR "Status of the e1000 card:\n");
	printk(KERN_ERR "\tStatus: %i\n", status);
	printk(KERN_ERR "\tMode: %s\n", (status & 1) ? "Full": "Half");
	printk(KERN_ERR "\tLink: %s\n", (status & 2) ? "UP" : "Down");
	printk(KERN_ERR "\tTransmission: %s\n", (status & 4) ? "Paused" : "Ok");
	printk(KERN_ERR "\tInterface: %s\n", (status & 3) == 3 ? "UP" : "Down");
	{
		int speed;
		static char* speed_s[4] = {
			"10Mb/s",
			"100Mb/s",
			"1000Mb/s",
			"1000Mb/s",
		};

		speed = (status & (3 << 6)) >> 6;
		printk(KERN_ERR "\tSpeed: Mode: %i -> %s\n", speed, speed_s[speed]);
	}
}

irqreturn_t	handler(int irq, void* dev_id)
{
	t_e1000_data* data;
	uint32_t intr;

	printk(KERN_ERR "INTERRUPT HANDLER\n");

	data = dev_id;
	intr = e1000_read_register(data, ICR_REG);

	if (!intr) {
		printk(KERN_ERR "There isn't interrupt to process\n");
		return IRQ_NONE;
	}

	printk(KERN_ERR "There is interrupt to process\n");

	data->waiting_intr |= intr;
	tasklet_schedule(&data->intr_tasklet);


	return IRQ_HANDLED;
}

static void intr_task(unsigned long pdata)
{
	t_e1000_data *data;
	struct net_device* dev = (struct net_device*) pdata;

	dev = card;

	printk(KERN_ERR "coucou Tasklet\n");

	data = netdev_priv(dev);

	if (!data->waiting_intr)
		return ;

	if (data->waiting_intr & (IMS_RX0 | IMS_RXT0)) {
		printk(KERN_ERR "Packet received\n");
		//e1000_read(dev);
	}

	if (data->waiting_intr & (IMS_TXDW | IMS_TXQE)) {
		printk(KERN_ERR "Packet has been sent or the transmit queue is empty\n");
	}

	if (data->waiting_intr & (IMS_LSC)) {
		printk(KERN_ERR "The link status changed\n");
		e1000_print_status(data);
	}

	data->waiting_intr = 0;
}

static int e1000_open(struct net_device *dev)
{
	t_e1000_data *data;
	int ret;

	printk(KERN_ERR "Open the e1000 card\n");

	data = netdev_priv(dev);

	if ((ret = init_hw(dev)))
		return ret;

	printk(KERN_ERR "Start queue\n");
	netif_start_queue(dev);
	return 0;
}

static int e1000_close(struct net_device *dev)
{
	printk(KERN_ERR "Close the e1000 card\n");
	netif_stop_queue(dev);
	uninit_hw(dev);
	return 0;
}

static int init_interrupt(struct net_device* dev)
{
	t_e1000_data *data;
	int ret;

	data = netdev_priv(dev);

	tasklet_init(&data->intr_tasklet, intr_task, (unsigned long)dev);

	ret = request_irq(data->irq,
			  handler,
			  IRQF_SHARED,
			  "e1000_driver",
			  data);

	if (!ret) {
		e1000_set_register(data, IMS_REG,
				IMS_TXDW |
				IMS_TXQE |
				IMS_LSC  |
				IMS_RX0  |
				IMS_RXT0);
	}
	else {
		printk(KERN_ERR "Not registered interrupt\n");
	}

	return ret;
}

static void uninit_interrupt(struct net_device* dev)
{
	t_e1000_data *data;

	data = netdev_priv(dev);
	free_irq(data->irq, data);
	tasklet_disable(&data->intr_tasklet);
}

static netdev_tx_t e1000_start_xmit(struct sk_buff *skb, struct net_device *dev)
{
	t_e1000_data* data;
	uint32_t tail;
	t_send_desc* tail_desc;


	printk(KERN_WARNING "e1000_start_xmit called\n");

	if (skb_shinfo(skb)->nr_frags)
	{
		printk(KERN_ERR "Packet fragmented\n");
		return NETDEV_TX_OK;
	}

	data = netdev_priv(dev);
	tail = e1000_read_register(data, TDT_REG);

	tail_desc = data->send.descriptors + tail;
	memcpy((void*)((uint32_t)data->send.buff + (tail * BUF_SIZE_BY_DESC)), skb->data, skb->len);
	printk(KERN_ERR "Transmit:\n\tTail: %i\n\tLen: %i\n",
			tail, skb->len);
	tail_desc->length = skb->len;
	tail_desc->command = (TX_EOP | TX_IFCS | TX_RS);
	tail_desc->status = 0;

	tail = (tail + 1) % NB_SND_DESC;

	e1000_set_register(data, TDT_REG, tail);


	return NETDEV_TX_OK;
}

static void e1000_read(struct net_device* dev)
{
	t_e1000_data *data;
	uint32_t tail;
	t_recv_desc* desc;
	struct sk_buff *skb;
	int i = 0;

	printk(KERN_ERR "Read a packet\n");
	data = netdev_priv(dev);
	tail = e1000_read_register(data, RDT_REG);
	tail = (tail + 1) % NB_RCV_DESC;
	desc = data->recv.descriptors + tail;

	printk(KERN_ERR "Tail: %i\n", tail);

	// for (; i < NB_RCV_DESC; ++i)
	// 	printk(KERN_ERR "Status of: %i: %i\n", i, data->recv.descriptors[i].status);

	if (!(desc->status & RX_EOP))
	{
		printk(KERN_WARNING "No packet to read\n");
		return ;
	}

	skb = dev_alloc_skb(desc->length);

	if (!skb) {
		if (printk_ratelimit())
			printk(KERN_ERR "Could not allocate socket buffer\n");
		return ;
	}
	uint32_t *ptr = (uint32_t*)((uint32_t)data->recv.buff + (tail * BUF_SIZE_BY_DESC));
	memcpy(skb_put(skb, desc->length), (const void*)((uint32_t)data->recv.buff + (tail * BUF_SIZE_BY_DESC)), desc->length);
	printk(KERN_ERR "--------------------------\n");
	for (i = 0; i < desc->length; ++i) {
		printk("%#x ", *ptr++);
	}
	printk(KERN_ERR "--------------------------\n");
	skb->dev = dev;
	skb->protocol = eth_type_trans(skb, dev);
	skb->ip_summed = CHECKSUM_UNNECESSARY;

	e1000_set_register(data, RDT_REG, tail);
	netif_rx(skb);
}

static int init_hw(struct net_device* dev)
{
	t_e1000_data *data;
	int ret;
	int i;

	data = netdev_priv(dev);

	printk(KERN_ERR "Inititalize HW\n");

	e1000_print_status(data);

	/* Take a look to the Intel Manual Developper
	 * section: 14.3
	 */

	/* Activate automatic speed detection
	 * it requires the ASDE and the SLU bit
	 * set in the control register
	 */

	e1000_set_register_preserve(data, CTRL_REG,
			CTRL_ASDE | CTRL_SLU);

	/*
	 * Activate automatic negociation
	 */

	e1000_unset_register_preserve(data, CTRL_REG,
			CTRL_LRST);

	e1000_unset_register_preserve(data, CTRL_REG,
			CTRL_PHYRST);

	/* Invert Los  of signal */
	e1000_unset_register_preserve(data, CTRL_REG, CTRL_ILOS);

	/*
	 * Desactivate flow control
	 */

	e1000_set_register(data, FCAL_REG,  0);
	e1000_set_register(data, FCAH_REG,  0);
	e1000_set_register(data, FCT_REG,   0);
	e1000_set_register(data, FCTTV_REG, 0);

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

	printk(KERN_ERR "HW initialized\n");
	e1000_print_status(data);


	printk(KERN_ERR "Set mac address\n");
	e1000_set_mac_address();

	printk(KERN_ERR "Init queues\n");
	if ((ret = init_queue(dev)))
		return ret;

	printk(KERN_ERR "Init interrupt\n");
	if ((ret = init_interrupt(dev)))
		return ret;

	return 0;

}

static void uninit_hw(struct net_device* dev)
{
	t_e1000_data* data;

	data = netdev_priv(dev);
	uninit_interrupt(dev);
	uninit_queue(dev);
	e1000_set_register(data, CTRL_REG, CTRL_RST);
}

static int init_queue(struct net_device* dev)
{
	t_e1000_data* data;
	int i;
	int err;

	data = netdev_priv(dev);

	if ((err = dma_set_mask(&data->pci->dev, DMA_BIT_MASK(32))))
		return err;

	/*
	 * use NB_RCV_DESC with BUF_SIZE_BY_DESC size
	 * for each
	 */

	data->recv.size_desc = sizeof(t_recv_desc) * NB_RCV_DESC;
	data->recv.size_desc = ALIGN(data->recv.size_desc, 4096);

	data->recv.descriptors = dma_alloc_coherent(&data->pci->dev,
						    data->recv.size_desc,
						    &data->recv.dma_desc,
						    GFP_KERNEL);


	data->recv.size_buff = BUF_SIZE_BY_DESC * NB_RCV_DESC;
	data->recv.size_buff = ALIGN(data->recv.size_desc, 4096);


	data->recv.buff = dma_alloc_coherent(&data->pci->dev,
					     data->recv.size_buff,
					     &data->recv.dma_buff,
					     GFP_KERNEL);

	if (!data->recv.descriptors || !data->recv.buff)
		return -ENOMEM;

	memset(data->recv.descriptors, 0, data->recv.size_desc);

	/* init the buffer address for each descriptor */
	for (i = 0; i < NB_RCV_DESC; ++i)
		data->recv.descriptors[i].address_l = data->recv.dma_buff + (i * BUF_SIZE_BY_DESC);

	/* Initialize the ring buffer (address, len, head, tail) */
	e1000_set_register(data, RDBAL_REG, data->recv.dma_desc);
	e1000_set_register(data, RDBAH_REG, 0);
	e1000_set_register(data, RDLEN_REG, data->recv.size_desc);
	e1000_set_register(data, RDH_REG, 0);
	e1000_set_register(data, RDT_REG, NB_RCV_DESC - 1);

	/* Set the size of the buffer in control register */
	e1000_unset_register_preserve(data, RCTL_REG, RCTL_BSIZE);
	e1000_unset_register_preserve(data, RCTL_REG, RCTL_BSEX);

	/*** TRANSMISSION SETUP ***/

	data->send.size_desc = sizeof(t_send_desc) * NB_SND_DESC;
	data->send.size_desc = ALIGN(data->send.size_desc, 4096);

	data->send.descriptors = dma_alloc_coherent(&data->pci->dev,
						    data->send.size_desc,
						    &data->send.dma_desc,
						    GFP_KERNEL);

	data->send.size_buff = BUF_SIZE_BY_DESC * NB_SND_DESC;
	data->send.size_buff = ALIGN(data->send.size_buff, 4096);

	data->send.buff = dma_alloc_coherent(&data->pci->dev,
					     data->send.size_buff,
					     &data->send.dma_buff,
					     GFP_KERNEL);

	if (!data->send.descriptors || !data->send.buff)
		return -ENOMEM;
	memset(data->send.descriptors, 0, data->send.size_desc);

	/* init the buffer address for each descriptor */
	for (i = 0; i < NB_SND_DESC; ++i)
		data->send.descriptors[i].address_l = data->send.dma_buff + (i * BUF_SIZE_BY_DESC);

	e1000_set_register(data, TDBAL_REG, data->send.dma_desc);
	e1000_set_register(data, TDBAH_REG, 0);
	e1000_set_register(data, TDLEN_REG, data->send.size_desc);
	e1000_set_register(data, TDH_REG, 0);
	e1000_set_register(data, TDT_REG, 0);

	/* enable transmitting */
	e1000_set_register_preserve(data, TCTL_REG, TCTL_ENABLE | TCTL_PAD_PACK | TCTL_CT);

	/* enable receiving */
	e1000_set_register_preserve(data, RCTL_REG, RCTL_ENABLE);

	e1000_print_status(data);

	return 0;
}

static void uninit_queue(struct net_device* dev)
{
	t_e1000_data* data;

	data = netdev_priv(dev);
	e1000_unset_register_preserve(data, RCTL_REG, RCTL_ENABLE);
	e1000_unset_register_preserve(data, TCTL_REG, TCTL_ENABLE);

	dma_free_coherent(&data->pci->dev,
			  data->recv.size_desc,
			  data->recv.descriptors,
			  data->recv.dma_desc);

	dma_free_coherent(&data->pci->dev,
			  data->recv.size_buff,
			  data->recv.buff,
			  data->recv.dma_buff);

	dma_free_coherent(&data->pci->dev,
			  data->send.size_desc,
			  data->send.descriptors,
			  data->send.dma_desc);

	dma_free_coherent(&data->pci->dev,
			  data->send.size_buff,
			  data->send.buff,
			  data->send.dma_buff);
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

	e1000_set_register(data, RAL_REG, *(uint32_t*)(&mac_address[0]));
	e1000_set_register(data, RAH_REG, *(uint16_t*)(&mac_address[4]));
	e1000_set_register_preserve(data, RAH_REG, RAH_AV);

	/* Activate Multi Promiscuous mode */
	e1000_set_register_preserve(data, RCTL_REG, RCTL_MPE);


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
	int err;
	t_e1000_data *data;

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
	memset(data, 0, sizeof(*data));
	data->bar = pci_ioremap_bar(dev, 0);
	data->netdev = card;
	data->pci = dev;
	data->irq = dev->irq; // interrupt;
	return 0;
}

void remove(struct pci_dev *dev)
{
	printk(KERN_WARNING "remove called\n");
	unload_hw();
	pci_release_region(dev, 0);
	pci_disable_device(dev);
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
