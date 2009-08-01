CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS = reader1	tree1

all:	${PROGS}

reader1.o:	reader1.c
	${CC} ${CFLAGS} -c $< -I/usr/include/libxml2

reader1:	reader1.o
	${CC} ${CFLAGS} $^ -o $@ -lxml2
    
tree1.o:	tree1.c
	${CC} ${CFLAGS} -c $< -I/usr/include/libxml2

tree1:	tree1.o
	${CC} ${CFLAGS} $^ -o $@ -lxml2

myzip:
	tar cvzf xmlTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
