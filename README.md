# Naive-data-structure

Demo try with naive data structure

Trying to create some navie data structures to show potential of using xrp.

----

Targets:

1. Modification with BPF-KV
   - add aggregation - 20% speedup
2. EasyList
3. SkipList
4. LinkedList

Current Problems:

1. Not fully understand disk-io process in the code implementation - solved
2. XRP transplant

Current Workflow:

1. Try to imitate the xrp implementation step-by-step
----

## Remarks

1. add insert-deletion : not best use case of XRP
2. nvme_handle_cqe / xrp_resubmit_level_count -> constrains
3. file system - ext4 -> find a file system
4. jump index -> other datastructures
5. moving computation closer to the device -> design complex problems
6. bypass network
