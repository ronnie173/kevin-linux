CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump *~

PROGS =	simple_http

all:	${PROGS}

simple_http.o:	simple_http.c
	${CC} ${CFLAGS} -c $<

simple_http:	simple_http.o
	${CC} ${CFLAGS} $^ -o $@ -lc

myzip:
	tar cvzf simple_http.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
