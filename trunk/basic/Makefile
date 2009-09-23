CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *~ *.stackdump

PROGS =	structTest sortTest strTest funcTest arrayTest byteTest getOptTest

all:	${PROGS}

structTest:	structTest.o
	${CC} ${CFLAGS} -o $@ structTest.o
    
sortTest:	sortTest.o
	${CC} ${CFLAGS} -o $@ sortTest.o

strTest:	strTest.o
	${CC} ${CFLAGS} -o $@ strTest.o

funcTest:	funcTest.o
	${CC} ${CFLAGS} -o $@ funcTest.o

arrayTest:	arrayTest.o
	${CC} ${CFLAGS} -o $@ arrayTest.o

byteTest:	byteTest.o
	${CC} ${CFLAGS} -o $@ byteTest.o

getOptTest:	getOptTest.o
	${CC} ${CFLAGS} -o $@ getOptTest.o

myzip:
	tar cvzf basic.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
