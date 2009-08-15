CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

CLEANFILES = core core.* *.core *.o temp.* *.out *.exe *.*~ *.stackdump

PROGS =	first_pcre json_util_test json-c_util_test

all:	${PROGS}

json.o:	json.c json.h
	${CC} ${CFLAGS} -c $<

json_helper.o:	json_helper.c json_helper.h
	${CC} ${CFLAGS} -c $<

jsonUtil.o:	jsonUtil.c jsonUtil.h json.c json.h
	${CC} ${CFLAGS} -c $<

json-cUtil.o:	json-cUtil.c json-cUtil.h
	${CC} ${CFLAGS} -I/usr/local/include/json -c $<

jsonUtilTest.o:	jsonUtilTest.c
	${CC} ${CFLAGS} -c $<

json_util_test:	jsonUtilTest.o jsonUtil.o json.o json_helper.o
	${CC} ${CFLAGS} $^ -o $@

json-cUtilTest.o:	json-cUtilTest.c
	${CC} ${CFLAGS} -I/usr/local/include/json -c $<

json-c_util_test:	json-cUtilTest.o json-cUtil.o
	${CC} ${CFLAGS} -L/usr/local/lib $^ -o $@ -ljson

first_pcre.o:	first_pcre.c json-cUtil.h
	${CC} ${CFLAGS} -I/usr/local/include/json -c $<

first_pcre:	first_pcre.o json-cUtil.o
	${CC} ${CFLAGS} -L/usr/local/lib $^ -o $@ -lpcre -ljson
    
myzip:
	tar cvzf pcreTest.tgz Makefile README *.c *.h

clean:
	rm -f ${PROGS} ${CLEANFILES}
