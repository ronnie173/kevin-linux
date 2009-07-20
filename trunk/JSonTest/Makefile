CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	qsortints sortTest quicksort

all:	${PROGS}

qsortints:	qsortints.o
	${CC} ${CFLAGS} -o $@ qsortints.o
    
sortTest:	sort.o
	${CC} ${CFLAGS} -o $@ sort.o

quicksort:	quicksort.o
	${CC} ${CFLAGS} -o $@ quicksort.o

myzip:
	tar cvzf sortTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
