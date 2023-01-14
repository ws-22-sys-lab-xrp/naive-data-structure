#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "helpers.h"
#include "db_types.h"

#define BLK_SIZE 512
#define BLK_SIZE_LOG 9
#define SCRATCH_SIZE 4096

// TODO: Need to check the number
#define SYS_READ_XRP 445

typedef unsigned long ptr__t;
typedef unsigned long uintptr_t;

#define aligned_alloca(align, size) (((uintptr_t)alloca((size) + (align)-1) + ((align)-1)) & ~(uintptr_t)((align)-1));

long lookup_bpf(int db_fd, int bpf_fd, struct Query *query, ptr__t index_offset)
{
    /* Set up buffers and query */
    int *buf = (int *)aligned_alloca(0x1000, 0x1000);
    int *scratch = (int *)aligned_alloca(0x1000, SCRATCH_SIZE);
    memset(buf, 0, 0x1000);
    memset(scratch, 0, 0x1000);

    struct ScatterGatherQuery *sgq = (struct ScatterGatherQuery *)scratch;
    sgq->keys[0] = query->key;
    sgq->n_keys = 1;

    /* Syscall to invoke BPF */
    long ret = syscall(SYS_READ_XRP, db_fd, buf, BLK_SIZE, index_offset, bpf_fd, scratch);

    printf("syscall Finish!\n\n");

    struct MaybeValue *maybe_v = &sgq->values[0];
    query->found = maybe_v->found;
    if (query->found)
    {
        printf("Found!\n\n");
        memcpy(&(query->value), &(maybe_v->value), sizeof(int));
    }
    return query->value;
}

int load_bpf_program(char *path)
{
    struct bpf_object *obj;
    int ret, progfd;

    ret = bpf_prog_load(path, BPF_PROG_TYPE_XRP, &obj, &progfd);
    if (ret)
    {
        printf("Failed to load bpf program\n");
        exit(1);
    }

    return progfd;
}
