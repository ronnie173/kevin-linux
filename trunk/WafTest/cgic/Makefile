CFLAGS=-g -Wall
CC=gcc
AR=ar
RANLIB=ranlib
LIBS=-L./ -lcgic

all: libcgic.a cgictest.cgi capture test_post.cgi hello.cgi simple_hello.cgi

install: libcgic.a
	cp libcgic.a /usr/local/lib
	cp cgic.h /usr/local/include
	@echo libcgic.a is in /usr/local/lib. cgic.h is in /usr/local/include.

libcgic.a: cgic.o cgic.h
	rm -f libcgic.a
	$(AR) rc libcgic.a cgic.o
	$(RANLIB) libcgic.a

#mingw32 and cygwin users: replace .cgi with .exe

cgictest.cgi: cgictest.o libcgic.a
	gcc cgictest.o -o cgictest.cgi ${LIBS}

test_post.cgi: test_post.o libcgic.a
	gcc test_post.o -o test_post.cgi ${LIBS}

hello.cgi: hello.o libcgic.a
	gcc hello.o -o hello.cgi ${LIBS}

simple_hello.cgi: simple_hello.o
	gcc simple_hello.o -o simple_hello.cgi

capture: capture.o libcgic.a
	gcc capture.o -o capture ${LIBS}

clean:
	rm -f *.o *.a cgictest.cgi capture

