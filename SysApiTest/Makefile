CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	MmapTest

all:	${PROGS}

MmapTest.o:	MmapTest.c
	${CC} ${CFLAGS} -c $<

MmapTest:	MmapTest.o
	${CC} ${CFLAGS} -L/usr/local/lib $^ -o $@

myzip:
	tar cvzf SysApiTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
