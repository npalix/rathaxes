#ifndef _WDF_HELLO_H_
# define _WDF_HELLO_H_

# include <ntddk.h>
# include <wdf.h>

//Prototypes of the DriverEntry point ( defined in <wdm.h> )
DRIVER_INITIALIZE DriverEntry;

NTSTATUS
WdfHelloEvtDeviceAdd(
    IN WDFDRIVER Driver,
    IN PWDFDEVICE_INIT DeviceInit
    );
#endif  // ! _WDF_HELLO_H_
