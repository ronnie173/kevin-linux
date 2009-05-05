CC = gcc
CFLAGS = -g -Wall
LIB_OBJS =

SUBDIRS = 
PROGS =	

all:
	@for i in $(SUBDIRS); do \
		make --directory=$$i; \
	done

clean:
	rm -f core *~

	@for i in $(SUBDIRS); do \
		make -C $$i clean || exit 1; \
	done
	
