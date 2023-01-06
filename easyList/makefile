CC = gcc
CFLAGS = -Wall -D_GNU_SOURCE -Wunused
LDLIBS = -pthread -lbpf -lm


all: easylist bpf

easylist: easylist.c easylist.h db_types.h helpers.o range.o parse.o create.o get.o

helpers.o: helpers.c helpers.h db_types.h

range.o: range.c range.h db_types.h parse.h db_types.h easylist.h helpers.h

parse.o: parse.c parse.h helpers.h

create.o: create.c create.h parse.h db_types.h easylist.h

get.o : get.c get.h db_types.h parse.h easylist.h

.PHONY: bpf
bpf:
	make -C xrp-bpf -f Makefile

.PHONY: clean
clean:
	rm -rf easylist *.o
	make -C xrp-bpf -f Makefile clean
