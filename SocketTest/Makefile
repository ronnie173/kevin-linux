CC = gcc
CFLAGS = -g -O0 -Wall $(INCS)
LIB_OBJS =

KERNEL_DIR = /usr/src/kernel
KERNEL_INC_DIR = $(KERNEL_DIR)/include

# INCS = -I$(KERNEL_INC_DIR)

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *~ *.stackdump

PROGS =	checkopts

all:	${PROGS}

checkopts:	checkopts.o
	${CC} ${CFLAGS} -o $@ $<

myzip:
	tar cvzf socket_test.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
