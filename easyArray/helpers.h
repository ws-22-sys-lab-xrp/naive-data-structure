#ifndef HELPERS_H
#define HELPERS_H

#include <linux/bpf.h>
#include <linux/lirc.h>
#include <linux/input.h>
#include <bpf/bpf.h>
#include <bpf/libbpf.h>

#include <errno.h>
#include <stdlib.h>
#include <stdio.h>
#include <alloca.h>
#include <stdint.h>
#include <math.h>

#include "db_types.h"

long lookup_bpf(int db_fd, int bpf_fd, struct Query *query, ptr__t index_offset);

int load_bpf_program(char *path);

#endif /* HELPERS_H */
