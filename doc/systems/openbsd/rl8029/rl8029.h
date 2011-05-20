#ifndef __RL8029_H__
# define __RL8029_H__

/* Define for more readable code */

#define	_R
#define _W
#define _RW
#define NOT_NE2K

/* PCI REGISTERS */

#define PCI_RL8029_VENDOR_PRODUCT		0x00
#define	PCI_RL8029_COMMAND_STATUS		0x04
#define	PCI_RL8029_RID_PIFR_SCR_BCR		0x08
#define PCI_RL8029_RES_LTR_HTR_RES		0x0c
#define PCI_RL8029_BAR				0x10
#define PCI_RL8029_SVID_SID			0x2c
#define PCI_RL8029_BROMBAR			0x30
#define PCI_RL8029_ILR_IPR_RES_RES		0x3c

extern const struct pci_known_product pci_known_products[];
extern const struct pci_known_vendor pci_known_vendors[];

/* NE2K REGISTERS */

#define	RL8029_CR				0x00
#define RL8029_REMOTE_DMA_PORT			0x10
#define RL8029_REMOTE_DMA_SIZE			0x08

// REGISTER FOR PAGE 1

#define RL8029_CLAD0			_R	0x01
#define	RL8029_PSTART_1			_W	0x01
#define RL8029_CLAD1			_R	0x02
#define	RL8029_PSTOP_1			_W	0x02
#define RL8029_BNRY			_RW	0x03
#define RL8029_TSR			_R	0x04
#define	RL8029_TPCR_1			_W	0x04
#define RL8029_NCR			_R	0x05
#define	RL8029_TBCR0			_W	0x05
#define RL8029_FIFO			_R	0x06
#define	RL8029_TBCR1			_W	0x06
#define RL8029_ISR			_RW	0x07
#define RL8029_CRDA0			_R	0x08
#define	RL8029_RSAR0			_W	0x08
#define RL8029_CRDA1			_R	0x09
#define	RL8029_RSAR1			_W	0x09
#define RL8029_8029ID0	NOT_NE2K	_R	0x0a
#define	RL8029_RBCR0			_W	0x0a
#define RL8029_8029ID1	NOT_NE2K	_R	0x0b
#define	RL8029_RBCR1			_W	0x0b
#define RL8029_RSR			_R	0x0c
#define	RL8029_RCR_1			_W	0x0c
#define RL8029_CNTR0			_R	0x0d
#define	RL8029_TCR_1			_W	0x0d
#define RL8029_CNTR1			_R	0x0e
#define	RL8029_DCR_1			_W	0x0e
#define RL8029_CNTR2			_R	0x0f
#define	RL8029_IMR_1			_W	0x0f

// REGISTER FOR PAGE 2

#define	RL8029_PAR0			_R	0x01
#define	RL8029_PSTART_2			_W	0x01
#define	RL8029_PAR1			_R	0x02
#define	RL8029_PSTOP_2			_W	0x02
#define	RL8029_PAR2			_R	0x03
#define	RL8029_PAR3			_R	0x04
#define	RL8029_TPCR_2			_W	0x04
#define	RL8029_PAR4			_R	0x05
#define	RL8029_PAR5			_R	0x06
#define	RL8029_CURR			_R	0x07
#define	RL8029_MAR0			_R	0x08
#define	RL8029_MAR1			_R	0x09
#define	RL8029_MAR2			_R	0x0a
#define	RL8029_MAR3			_R	0x0b
#define	RL8029_MAR4			_R	0x0c
#define	RL8029_RCR_2			_W	0x0c
#define	RL8029_MAR5			_R	0x0d
#define	RL8029_TCR_2			_W	0x0d
#define	RL8029_MAR6			_R	0x0e
#define	RL8029_DCR_2			_W	0x0e
#define	RL8029_MAR7			_R	0x0f
#define	RL8029_IMR_2			_W	0x0f

// REGISTER FOR PAGE 3

#define RL8029_9346CR	NOT_NE2K	_RW	0x01
#define RL8029_CONFIG0	NOT_NE2K	_R	0x03
#define RL8029_CONFIG2	NOT_NE2K	_RW	0x05
#define RL8029_CONFIG3	NOT_NE2K	_RW	0x06
#define RL8029_HLTCLK	NOT_NE2K	_W	0x09
#define RL8029_ASID0	NOT_NE2K	_R	0x0e
#define RL8029_ASID1	NOT_NE2K	_R	0x0f

#endif /*__RL8029_H__*/
