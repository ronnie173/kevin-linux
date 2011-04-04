CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	sqliteTest insertTest

all:	${PROGS}

test.o:	test.c
	${CC} ${CFLAGS} -c $<

sqliteTest:	test.o
	${CC} ${CFLAGS} -L/usr/local/lib $^ -o $@ -lsqlite3

insertTest.o:	insertTest.c insertTest.h
	${CC} ${CFLAGS} -c $<

insertTest:	insertTest.o
	${CC} ${CFLAGS} -L/usr/local/lib $^ -o $@ -lsqlite3

myzip:
	tar cvzf sqliteTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
