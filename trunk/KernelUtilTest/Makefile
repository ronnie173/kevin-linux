CC = gcc
CFLAGS = -g -Wall $(INCS)
LIB_OBJS =

KERNEL_DIR = /usr/src/kernel
KERNEL_INC_DIR = $(KERNEL_DIR)/include

INCS = -I$(KERNEL_INC_DIR)

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *~ *.stackdump

PROGS =	ListTest

all:	${PROGS}

ListTest:	ListTest.o
	${CC} ${CFLAGS} -o $@ ListTest.o

myzip:
	tar cvzf kernel_util_test.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
