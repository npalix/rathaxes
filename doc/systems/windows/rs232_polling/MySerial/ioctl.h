#ifndef IOCTL_H_
# define IOCTL_H_

VOID EvtIoIoctl(
    IN WDFQUEUE  Queue,
    IN WDFREQUEST  Request,
    IN size_t  OutputBufferLength,
    IN size_t  InputBufferLength,
    IN ULONG  IoControlCode
    );

#endif /* IOCTL_H_ */
