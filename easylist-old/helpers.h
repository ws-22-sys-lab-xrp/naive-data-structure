#ifndef HELPERS_H
#define HELPERS_H

#include "db_types.h"

long lookup_bpf(int db_fd, int bpf_fd, struct Query *query, ptr__t index_offset);

int load_bpf_program(char *path);

#endif /* HELPERS_H */