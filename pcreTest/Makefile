CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	first_pcre json_util

all:	${PROGS}

json.o:	json.c json.h
	${CC} ${CFLAGS} -c $<

jsonUtil.o:	jsonUtil.c jsonUtil.h json.c json.h
	${CC} ${CFLAGS} -c $<

json_util:	jsonUtil.o json.o
	${CC} ${CFLAGS} $^ -o $@

first_pcre:	first_pcre.o
	${CC} ${CFLAGS} -o $@ first_pcre.o -lpcre
    
myzip:
	tar cvzf pcreTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
