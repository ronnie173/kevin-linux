CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS = json_test

all:	${PROGS}

json.o:	json.c json.h
	${CC} ${CFLAGS} -c $<
    
json_helper.o:	json_helper.c json_helper.h
	${CC} ${CFLAGS} -c $<

json_test.o:	json_test.c
	${CC} ${CFLAGS} -c $<

json_test:	json_test.o json.o json_helper.o
	${CC} ${CFLAGS} $^ -o $@  

myzip:
	tar cvzf JSonTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
