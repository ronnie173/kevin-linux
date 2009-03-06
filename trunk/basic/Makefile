CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~

PROGS =	structTest

all:	${PROGS}

structTest:	structTest.o
	${CC} ${CFLAGS} -o $@ structTest.o
    
myzip:
	tar cvzf structTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
