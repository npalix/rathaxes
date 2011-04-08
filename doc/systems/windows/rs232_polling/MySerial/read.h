#ifndef READ_H_
# define READ_H_

VOID
EvtIoRead(
    IN WDFQUEUE         Queue,
    IN WDFREQUEST       Request,
    IN size_t           Length
    );

VOID
EvtIoDefault(
	IN WDFQUEUE  Queue,
	IN WDFREQUEST  Request
	);
#endif //! READ_H_
