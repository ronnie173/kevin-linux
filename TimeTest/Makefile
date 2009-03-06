CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe

PROGS =	gmtTest

all:	${PROGS}

gmtTest:	gmtTest.o
	${CC} ${CFLAGS} -o $@ gmtTest.o
    
myzip:
	tar cvzf gmtTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
