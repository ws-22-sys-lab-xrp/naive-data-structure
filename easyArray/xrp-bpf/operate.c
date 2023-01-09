#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>
#include "../db_types.h"

#define IN_PROCESS 0;
#define REACH_END 1;

#define EBPF_INT_SIZE 4

char LICENSE[] SEC("license") = "GPL";

// static __inline void set_context_next_index(struct bpf_xrp *context, struct ScatterGatherQuery *query)
// {
//     query->current_index += 1;
//     query->state_flags = 0;
//     if (query->current_index >= query->n_keys || query->current_index >= SG_KEYS)
//     {
//         context->done = 1;
//         context->next_addr[0] = 0;
//         context->size[0] = 0;
//     }
//     else
//     {
//         context->next_addr[0] = query->root_pointer;
//         context->size[0] = BLK_SIZE;
//     }
// }

// static __inline void set_context_next_iteration(struct bpf_xrp *context, struct ScatterGatherQuery *query)
// {
//     query->current_iteration += 1;
//     query->state_flag = 0;
//     if (query->current_iteration >= query->iteration)
//     {
//         context->done = 1;
//         context->next_addr[0] = 0;
//         context->size[0] = 0;
//     }
//     else
//     {
//         // TODO: need to check again
//         context->next_addr[0] = 0;
//         context->size[0] = 0;
//     }
// }

#define EBPF_CONTEXT_MASK SG_KEYS - 1

SEC("operate")
unsigned int easylist(struct bpf_xrp *context)
{
    struct ScatterGatherQuery *query = (struct ScatterGatherQuery *)context->scratch;
    int current_iteration = query->current_iteration;
    int current_array_index = query->current_array_index;
    // Extension to searching more elements - range
    // int *curr_idx = &query->current_index;

    // Fetch the target value
    // TODO: find out the relating address operations
    int *location = (query->array + current_array_index * EBPF_INT_SIZE);
    int value = *location;

    // TODO: pass out the new index things - By using context->data
    // Case 1 - End Term Processing -> fetch the value
    // Return the value
    if (current_iteration >= query->iteration)
    {
        query->state_flags = REACH_END;
        context->done = 1;
        // Saving the result to context->date
        memcpy(location, context->data, sizeof(int));
    }

    // Case 2 - Continue Processing

    int new_index = (value * 2) % (query->array_length);
    query->current_array_index = new_index;

    return 0;
}

static __inline int function_name()
{
    return 0;
}

static __inline int fetch_value(int index)
{

    return 0;
}