#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/pci.h>

#include <sound/core.h>
#include <sound/initval.h>

MODULE_DESCRIPTION("Minimal test driver for ac97 compliant sound cards");
MODULE_AUTHOR("Louis Opter <louis@lse.epitech.net>");
MODULE_LICENSE("GPL");

static struct pci_device_id	rtx_ac97_pci_tbl[] = {
	{ PCI_VDEVICE(INTEL, PCI_DEVICE_ID_INTEL_82801AA_5), },	/* 82801AA */
	{ }
};

MODULE_DEVICE_TABLE(pci, rtx_ac97_pci_tbl);

struct	rtx_ac97 {
	struct pci_dev	*pdev;
	struct snd_card	*card;
};

/*
 * The macro used below are defined in sound/initval.h
 */
static int	rtx_ac97_index[SNDRV_CARDS] = SNDRV_DEFAULT_IDX; /* Index 0-MAX */
static char	*rtx_ac97_id[SNDRV_CARDS] = SNDRV_DEFAULT_STR; /* ID for this card */
static int	rtx_ac97_enable[SNDRV_CARDS] = SNDRV_DEFAULT_ENABLE_PNP; /* Enable this card */

/*
 * Our destructor.
 */
static int	rtx_ac97_free(struct rtx_ac97 *chip)
{
	printk(KERN_INFO "rtx_ac97: destroying chip=0x%p.\n", chip);
	pci_disable_device(chip->pdev);
	kfree(chip);
	return (0);
}

/*
 * Callback called installed by rtx_ac97_create and called by snd_card_free().
 */
static int	rtx_ac97_dev_free(struct snd_device *sdev)
{
	return (rtx_ac97_free((struct rtx_ac97 *)sdev->device_data));
}

/*
 * Use __devinit to place it in the same section as rtx_ac97_probe() (see
 * below).
 */
static int __devinit	rtx_ac97_create(struct snd_card *card,
					struct pci_dev *pdev)
{
	static struct snd_device_ops	ops = {
		.dev_free = rtx_ac97_dev_free
	};
	struct rtx_ac97			*chip;
	int				err;

	err = pci_enable_device(pdev);
	if (err < 0)
		return (err);

	chip = kzalloc(sizeof(*chip), GFP_KERNEL);
	if (!chip) {
		err = -ENOMEM;
		goto fail;
	}
	chip->pdev = pdev;
	chip->card = card;

	/* Allocate PCI stuff here */

	/*
	 * create a "fake" lowlevel (custom) device to have a dtor for our chip
	 * structure:
	 */
	err = snd_device_new(card, SNDRV_DEV_LOWLEVEL, chip, &ops);
	if (err < 0)
		goto fail;

	/*
	 * registration of device structure this seems needed for alsa/linux
	 * internal stuff:
	 */
	snd_card_set_dev(card, &pdev->dev);

	printk(KERN_INFO "rtx_ac97: chip=0x%p created.\n", chip);

	/* attach ourselves to the card instance: */
	card->private_data = chip;

	return (0);

fail:
	if (chip)
		rtx_ac97_free(chip);
	else
		pci_disable_device(pdev);
	return (err);
}

/*
 * The ALSA drivers guide says to use __dev{init,exit} because PCI devices can
 * be hot-plugged.
 *
 * If you look into linux/init.h you can see that __dev{init,exit} place the
 * code in a different ELF section.
 */
static int __devinit	rtx_ac97_probe(struct pci_dev *pdev,
				       const struct pci_device_id *pdev_id)
{
	static int	dev_idx = 0;
	struct snd_card	*card;
	int		err;

	/* 1. check and increment the device index. XXX What is this? */
	if (dev_idx >= SNDRV_CARDS)
		return (-ENODEV);
	if (!rtx_ac97_enable[dev_idx]) {
		dev_idx++;
		return (-ENOENT);
	}

	/* 2. create a card instance. */
	err = snd_card_create(rtx_ac97_index[dev_idx], rtx_ac97_id[dev_idx], THIS_MODULE, 0, &card);
	if (err < 0)
		return (err);

	/* 3. create our stuff. */
	err = rtx_ac97_create(card, pdev);
	if (err < 0)
		goto fail;

	/* 4. set driver id and name strings. Visible in /proc/asound/cards */
	strlcpy(card->driver, "rtx_ac97", sizeof(card->driver));
	strlcpy(card->shortname, "My first AC'97 driver", sizeof(card->shortname));
	strlcpy(card->longname, "My first AC'97 driver", sizeof(card->longname));

	/* 5. create other components mixer. midi. etc. */

	/* 6. register the card instance. */
	err = snd_card_register(card);
	if (err < 0)
		goto fail;

	/*
	 * 7. Finally set the card instance private data in the struct pci_dev
	 * and return.
	 */
	pci_set_drvdata(pdev, card);
	dev_idx++;

	printk(KERN_INFO "rtx_ac97: probe successful: chip=0x%p.\n", card->private_data);
	return (0);

fail:
	printk(KERN_WARNING "rtx_ac97: probe failed.\n");

	snd_card_free(card);

	return (err);
}

static void __devexit	rtx_ac97_remove(struct pci_dev *pdev)
{
	printk(KERN_INFO "rtx_ac97: removing: chip=0x%p.\n", ((struct snd_card *)(pci_get_drvdata(pdev)))->private_data);
	snd_card_free(pci_get_drvdata(pdev));
	pci_set_drvdata(pdev, NULL);
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
