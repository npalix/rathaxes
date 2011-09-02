SRCS=hello.c
OBJS=$(SRCS:.c=.o)
KMOD=my_mod
CFLAGS+= -D_KERNEL -I/sys

all:	$(OBJS)

clean:
	rm -f $(OBJS) $(KMOD)

load:
	modload -o $(KMOD) -e$(KMOD) $(OBJS)

unload:
	modunload -n $(KMOD)
