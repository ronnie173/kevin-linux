CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

SUBDIRS = basic TimeTest
PROGS =	structTest sortTest

all:
	@for i in $(SUBDIRS); do \
		make --directory=$$i; \
	done

myzip:
	tar cvzf kevin-linux.tgz *

clean:
	@for i in $(SUBDIRS); do \
		make -C $$i clean || exit 1; \
	done
	
