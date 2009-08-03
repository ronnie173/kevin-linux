CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	tester

all:	${PROGS}

tester: hashtable.o tester.o hashtable_itr.o
	${CC} ${CFLAGS} -O -lm -o tester hashtable.o hashtable_itr.o tester.o

tester.o:	tester.c
	${CC} ${CFLAGS} -O -c tester.c -o tester.o

hashtable.o:	hashtable.c
	${CC} ${CFLAGS} -O -c hashtable.c -o hashtable.o

hashtable_itr.o: hashtable_itr.c
	${CC} ${CFLAGS} -O -c hashtable_itr.c -o hashtable_itr.o

clean:
	rm -f ${PROGS} ${CLEANFILES}
