#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

MODULE_DESCRIPTION("Minimal test driver for ac97 compliant sound cards");
MODULE_AUTHOR("Louis Opter <louis@lse.epitech.net>");
MODULE_LICENSE("GPL");

static struct pci_device_id	rtx_ac97_pci_tbl[] = {
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_82801AA_5), },	/* 82801AA */
	{ }
};

MODULE_DEVICE_TABLE(pci, rtx_ac97_pci_tbl);

static int __devinit	rtx_ac97_probe(struct pci_dev *pdev,
				       const struct pci_device_id *pdev_id)
{
	printk(KERN_INFO "rtx_ac97: probe.\n");
	return (-ENODEV);
}

static void __devexit	rtx_ac97_remove(struct pci_dev *pdev)
{
}

static struct pci_driver	rtx_ac97_driver = {
	.name = "rtx_ac97",
	.id_table = rtx_ac97_pci_tbl,
	.probe = rtx_ac97_probe,
	.remove = rtx_ac97_remove
};

static int __init	rtx_ac97_init(void)
{
	printk(KERN_INFO "rtx_ac97: driver registred.\n");
	return (pci_register_driver(&rtx_ac97_driver));
}

static void __exit	rtx_ac97_exit(void)
{
	pci_unregister_driver(&rtx_ac97_driver);
	printk(KERN_INFO "rtx_ac97: driver unregistred.\n");
}

module_init(rtx_ac97_init);
module_exit(rtx_ac97_exit);
