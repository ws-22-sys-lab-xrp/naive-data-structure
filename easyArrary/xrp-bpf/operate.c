#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include <string.h>
#include "../db_types.h"

#define IN_PROCESS 0;
#define REACH_END 1;

#define EBPF_INT_SIZE 4

char LICENSE[] SEC("license") = "GPL";

#define EBPF_CONTEXT_MASK SG_KEYS - 1

SEC("operate")
unsigned int easylist(struct bpf_xrp *context)
{
    struct ScatterGatherQuery *query = (struct ScatterGatherQuery *)context->scratch;
    unsigned int current_iteration = query->current_iteration;
    unsigned int current_array_index = query->current_array_index;
    // Extension to searching more elements - range
    // int *curr_idx = &query->current_index;

    // Fetch the target value
    // find out the relating address operations
    unsigned long location = (query->array + current_array_index * EBPF_INT_SIZE);
    int value;
    bpf_probe_read_kernel(&value, sizeof(int), (void *)location);

    // pass out the new index things - By using context->data
    // Case 1 - End Term Processing -> fetch the value
    // Return the value
    if (current_iteration >= query->iteration)
    {
        query->state_flags = REACH_END;
        context->done = 1;
        struct MaybeValue *mv = &query->values[0];
        mv->found = 1;
        // Saving the result to context->date
        memcpy(&(mv->value), context->data, sizeof(int));
    }

    // Case 2 - Continue Processing

    unsigned int new_index = (value * 2) % (query->array_length);
    query->current_array_index = new_index;

    return 0;
}
