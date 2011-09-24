CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	ldev testpcap1

all:	${PROGS}

ldev.o:	ldev.c
	${CC} ${CFLAGS} -c $<

testpcap1.o:	testpcap1.c
	${CC} ${CFLAGS} -c $<

ldev:	ldev.o
	${CC} ${CFLAGS} -L/usr/local/lib $^ -o $@ -lpcap

testpcap1:	testpcap1.o
	${CC} ${CFLAGS} -L/usr/local/lib $^ -o $@ -lpcap

myzip:
	tar cvzf pcaptest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
