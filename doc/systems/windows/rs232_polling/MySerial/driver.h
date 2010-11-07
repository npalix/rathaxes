#ifndef DRIVER_H_
# define DRIVER_H_

# include <ntddk.h>
# include <wdf.h>

# define DRIVER_NAME "[MySerial]"

/*++
DRIVER_INITIALIZE est un typedef sur le prototype
du point d'entrée du driver.
NTSTATUS DRIVER_INITIALIZE(_DRIVER_OBJECT*, PUNICODE_STRING)
--*/
DRIVER_INITIALIZE DriverEntry;

NTSTATUS
EvtDeviceAdd(
    IN WDFDRIVER		Driver,
    IN PWDFDEVICE_INIT	DeviceInit
    );
#endif //! DRIVER_H
