#ifndef PNP_H_
# define PNP_H_

NTSTATUS
EvtPrepareHardware(
		WDFDEVICE Device,
		WDFCMRESLIST Resources,
		WDFCMRESLIST ResourcesTranslated
		);

NTSTATUS
EvtReleaseHardware(
    IN  WDFDEVICE Device,
    IN  WDFCMRESLIST ResourcesTranslated
    );
#endif //!PNP_H_
