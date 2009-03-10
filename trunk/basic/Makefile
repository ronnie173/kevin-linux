CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~

PROGS =	structTest sortTest strTest

all:	${PROGS}

structTest:	structTest.o
	${CC} ${CFLAGS} -o $@ structTest.o
    
sortTest:	sortTest.o
	${CC} ${CFLAGS} -o $@ sortTest.o

strTest:	strTest.o
	${CC} ${CFLAGS} -o $@ strTest.o

myzip:
	tar cvzf basic.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
