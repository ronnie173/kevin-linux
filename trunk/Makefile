CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

SUBDIRS = basic TimeTest JSonTest
PROGS =	structTest sortTest

all:
	@for i in $(SUBDIRS); do \
		make --directory=$$i; \
	done

myzip:
	tar cvzf kevin-linux.tgz *

clean:
	rm -f core *~

	@for i in $(SUBDIRS); do \
		make -C $$i clean || exit 1; \
	done
	
