#ifndef WRITE_H_
# define WRITE_H_

VOID
EvtIoWrite(
    IN WDFQUEUE         Queue,
    IN WDFREQUEST       Request,
    IN size_t            Length
    );

#endif //!WRITE_H_
