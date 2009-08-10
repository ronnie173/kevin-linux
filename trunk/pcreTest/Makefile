CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	first_pcre json_util_test

all:	${PROGS}

json.o:	json.c json.h
	${CC} ${CFLAGS} -c $<

json_helper.o:	json_helper.c json_helper.h
	${CC} ${CFLAGS} -c $<

jsonUtil.o:	jsonUtil.c jsonUtil.h json.c json.h
	${CC} ${CFLAGS} -c $<

jsonUtilTest.o:	jsonUtilTest.c
	${CC} ${CFLAGS} -c $<

json_util_test:	jsonUtilTest.o jsonUtil.o json.o json_helper.o
	${CC} ${CFLAGS} $^ -o $@

first_pcre.o:	first_pcre.c jsonUtil.h
	${CC} ${CFLAGS} -c $<

first_pcre:	first_pcre.o jsonUtil.o json.o json_helper.o
	${CC} ${CFLAGS} $^ -o $@ -lpcre
    
myzip:
	tar cvzf pcreTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
