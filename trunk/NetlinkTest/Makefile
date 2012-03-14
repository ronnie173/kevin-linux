CC = gcc
CFLAGS = -g -O0 -Wall $(INCS)
LIB_OBJS =

KERNEL_DIR = /usr/src/kernel
KERNEL_INC_DIR = $(KERNEL_DIR)/include

# INCS = -I$(KERNEL_INC_DIR)

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *~ *.stackdump

PROGS =	user_space_write user_space_read

all:	${PROGS}

user_space_write:	user_space_write.o
	${CC} ${CFLAGS} -o $@ $<

user_space_read:	user_space_read.o
	${CC} ${CFLAGS} -o $@ $<

myzip:
	tar cvzf netlink_test.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
