#include <sys/param.h>
#include <sys/systm.h>
#include <sys/mbuf.h>
#include <sys/errno.h>
#include <sys/ioctl.h>
#include <sys/syslog.h>
#include <sys/socket.h>
#include <sys/device.h>

#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <net/if_media.h>

#ifdef INET
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <netinet/in_var.h>
#include <netinet/ip.h>
#include <netinet/if_ether.h>
#endif

#include <machine/bus.h>
#include <machine/intr.h>

#include <dev/pci/pcireg.h>
#include <dev/pci/pcivar.h>
#include <dev/pci/pcidevs.h>

#include <dev/mii/miivar.h>

#include "rl8029.h"

struct rl8029_softc {
	struct device sc_dev;

	void *sc_ih;	/*Interrupt handle*/

	bus_space_tag_t sc_bus_tag;
	bus_space_handle_t sc_bus_bh;	

	struct arpcom sc_arpcom;
	struct mii_data sc_mii;

	int sc_enabled;
};

void rl8029_reset(struct rl8029_softc *sc);
void rl8029_init(struct rl8029_softc *sc);
void rl8029_disable(struct rl8029_softc *sc);
void rl8029_start(struct ifnet *ifp);
void rl8029_stop(struct rl8029_softc *sc);
int rl8029_enable(struct rl8029_softc *sc);
int rl8029_ioctl(struct ifnet *ifp, u_long cmd, caddr_t data);
void rl8029_watchdog(struct ifnet *ifp);
int rl8029_mediachange(struct ifnet *ifp);
void rl8029_mediastatus(struct ifnet *ifp, struct ifmediareq *ifmr);
void rl8029_config(struct rl8029_softc *sc);
int rl8029_match(struct device *, void *, void *);
void rl8029_attach(struct device *, struct device *, void *);
int is_rl8029(struct pci_attach_args *pa);
int rl8029_intr(void *arg);

struct cfattach rln_ca = {
	sizeof(struct rl8029_softc), rl8029_match, rl8029_attach
};

struct cfdriver rln_cd = {
	NULL, "rln", DV_IFNET
};

int
is_rl8029(struct pci_attach_args *pa)
{
	if (PCI_VENDOR(pa->pa_id) == PCI_VENDOR_REALTEK &&
	    PCI_PRODUCT(pa->pa_id) == PCI_PRODUCT_REALTEK_RT8029)
		return 1;
	return 0;
}
	
int
rl8029_match(struct device *parent, void *match, void *aux)
{
	struct pci_attach_args *pa = aux;

	return is_rl8029(pa);
}

void
rl8029_config(struct rl8029_softc *sc)
{
	struct ifnet *ifp = &sc->sc_arpcom.ac_if;

	bcopy(sc->sc_dev.dv_xname, ifp->if_xname, IFNAMSIZ);
	ifp->if_softc = sc;
	ifp->if_start = rl8029_start;
	ifp->if_ioctl = rl8029_ioctl;
	if (!ifp->if_watchdog)
		ifp->if_watchdog = rl8029_watchdog;
	ifp->if_flags = 
		IFF_BROADCAST | IFF_SIMPLEX | 
		IFF_NOTRAILERS | IFF_MULTICAST;
	IFQ_SET_READY(&ifp->if_snd);

	ifp->if_capabilities = IFCAP_VLAN_MTU;

	printf(", adress %s", ether_sprintf(sc->sc_arpcom.ac_enaddr));

	ifmedia_init(&sc->sc_mii.mii_media, 0, rl8029_mediachange, rl8029_mediastatus);
	ifmedia_add(&sc->sc_mii.mii_media, IFM_ETHER|IFM_MANUAL, 0, NULL);
	ifmedia_set(&sc->sc_mii.mii_media, IFM_ETHER| IFM_MANUAL);

	if_attach(ifp);
	ether_ifattach(ifp);

	return ;
}

void 
rl8029_attach(struct device *parent, struct device *self, void *aux)
{
	struct rl8029_softc *psc = (struct rl8029_softc *)self;
	struct pci_attach_args *pa = aux;
	pci_chipset_tag_t pc = pa->pa_pc;
	bus_size_t iosize;
	const char *intrstr;
	pci_intr_handle_t ih;
	unsigned char reg;
		
	if (pci_mapreg_map(pa, PCI_RL8029_BAR, PCI_MAPREG_TYPE_IO, 0,
	    &psc->sc_bus_tag, &psc->sc_bus_bh, NULL, &iosize, 0)) {
		printf("rl8029 can't map i/o space\n");
		return ;
	}

	psc->sc_enabled = 1;

	if (pci_intr_map(pa, &ih)) {
		printf(":couldn't map interrupt\n");
		bus_space_unmap(psc->sc_bus_tag, psc->sc_bus_bh, iosize);
		return ;
	}

	intrstr = pci_intr_string(pc, ih);
	psc->sc_ih = pci_intr_establish(pc, ih, IPL_NET, rl8029_intr, 
		  psc, psc->sc_dev.dv_xname);
	if (psc->sc_ih == NULL) {
		printf("rl8029 can't establish interrupt\n");
		return ;
	}
	if (psc->sc_enabled)
		printf(": %s\n", intrstr);
	else
		printf(": toto %s\n", intrstr);
	
	printf("info : %.2x %.2x\n", psc->sc_bus_tag, psc->sc_bus_bh);
	printf("here\n");
	reg = bus_space_read_1(pc, psc->sc_bus_bh, RL8029_8029ID0);
	printf("reg :%.2x\n", reg);
	reg = bus_space_read_1(pc, psc->sc_bus_bh, RL8029_8029ID1);
	printf("reg :%.2x\n", reg);
	
	rl8029_config(psc);
}

int
rl8029_intr(void *arg)
{
	struct rl8029_softc *psc = (struct rl8029_softc *) arg;
	printf(": intr\n");
	psc->sc_enabled = 0;
	return 0;
}

void
rl8029_start(struct ifnet *ifp)
{
	printf("START\n");
	return ;
}

void
rl8029_stop(struct rl8029_softc *sc)
{
	printf("STOP\n");
}

void
rl8029_disable(struct rl8029_softc *sc)
{
	printf("DISABLE\n");
	return ;
}

int
rl8029_enable(struct rl8029_softc *sc)
{
	printf("ENABLE\n");
	return 0;
}

int	
rl8029_ioctl(struct ifnet *ifp, u_long cmd, caddr_t data)
{
	struct rl8029_softc *sc = ifp->if_softc;
	struct ifaddr *ifa = (struct ifaddr *) data;
	struct ifreq *ifr = (struct ifreq *) data;
	int s, error = 0;

	s = splnet();

	switch (cmd) {
		case SIOCSIFADDR:
			printf("SIOCDIFADDR\n");
			if ((error = rl8029_enable(sc)) != 0)
				break ;
			ifp->if_flags |= IFF_UP;
			switch (ifa->ifa_addr->sa_family) {
				case AF_INET:
					rl8029_init(sc);
					arp_ifinit(&sc->sc_arpcom, ifa);
					break ;
				default:
					rl8029_init(sc);
					break ;
			}
			break ;
		case SIOCSIFMTU:
			printf("SIOCSIFMTU\n");
			if (ifr->ifr_mtu > ETHERMTU || ifr->ifr_mtu < ETHERMIN) {
				error = EINVAL;
			} else if (ifp->if_mtu != ifr->ifr_mtu) {
				ifp->if_mtu = ifr->ifr_mtu;
			}
			break ;
		case SIOCSIFFLAGS:
			printf("SIOCIFFLAGS\n");
			if ((ifp->if_flags & IFF_UP) == 0 &&
		    	    (ifp->if_flags & IFF_RUNNING) != 0) {
				rl8029_stop(sc);
				ifp->if_flags &= ~IFF_RUNNING;
				rl8029_disable(sc);
			} else if ((ifp->if_flags & IFF_UP) != 0 &&
		    		   (ifp->if_flags & IFF_RUNNING) == 0) {
				if ((error = rl8029_enable(sc)) != 0)
					break;
				rl8029_init(sc);
			} else if ((ifp->if_flags & IFF_UP) != 0) {
				rl8029_stop(sc);
				rl8029_init(sc);
			}
			break ;
		case SIOCADDMULTI:
		case SIOCDELMULTI:
			printf("SIOCXXXMULTI\n");
			if (sc->sc_enabled == 0) {
				error = EIO;
				break ;
			}
			if (error == ENETRESET) {
				if (ifp->if_flags & IFF_RUNNING) {
					rl8029_stop(sc);			
					rl8029_init(sc);
				}
				error = 0;
			}
			break ;
		case SIOCGIFMEDIA:
		case SIOCSIFMEDIA:
			printf("SIOCXIFMEDIA\n");
			error = ifmedia_ioctl(ifp, ifr, &sc->sc_mii.mii_media, cmd);
			break;
		default:
			printf("DEFAULT\n");
			error = ether_ioctl(ifp, &sc->sc_arpcom, cmd, data);
	}

	splx(s);
	return error;
}

void
rl8029_watchdog(struct ifnet *ifp)
{
	struct rl8029_softc *sc = ifp->if_softc;

	log(LOG_ERR, "%s: device timeout\n", sc->sc_dev.dv_xname);
	++sc->sc_arpcom.ac_if.if_oerrors;
	rl8029_reset(sc);
	return ;
}

int
rl8029_mediachange(struct ifnet *ifp)
{
	printf("MEDIA CHANGE\n");
	return 0;
}

void
rl8029_mediastatus(struct ifnet *ifp, struct ifmediareq *ifmr)
{
	printf("MEDIA STATUS\n");
	return ;
}

void
rl8029_init(struct rl8029_softc *sc)
{
	printf("MEDIA INIT\n");
	return ;
}

void
rl8029_reset(struct rl8029_softc *sc)
{
	int	s;

	s = splnet();
	rl8029_stop(sc);
	rl8029_init(sc);
	splx(s);
}

