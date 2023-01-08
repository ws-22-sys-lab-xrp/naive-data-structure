#include <linux/bpf.h>
#include <bpf/bpf_helpers.h>

char LICENSE[] SEC("license") = "GPL";

SEC("operate")
unsigned int easylist(struct bpf_xrp *context)
{
    struct ScatterGatherQuery *query = (struct ScatterGatherQuery *)context->scratch;

    return 0;
}

static __inline int function_name()
{
    return 0;
}