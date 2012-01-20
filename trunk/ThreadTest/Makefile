CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *~ *.stackdump

PROGS =	ShareParam LocalVarParam

all:	${PROGS}

ShareParam:	ShareParam.o
	${CC} ${CFLAGS} -o $@ ShareParam.o -lpthread

LocalVarParam:	LocalVarParam.o
	${CC} ${CFLAGS} -o $@ LocalVarParam.o -lpthread
    
myzip:
	tar cvzf ThreadTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
