CC = gcc
CFLAGS = -Wall -D_GNU_SOURCE -Wunused
LDLIBS = -pthread -lbpf -lm


all: easylist bpf

easylist: easylist.c

.PHONY: bpf
bpf:
	make -C xrp-bpf -f Makefile

.PHONY: clean
clean:
	rm -rf simplekv *.o
	make -C xrp-bpf -f Makefile clean
