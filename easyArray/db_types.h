#ifndef DB_TYPES_H
#define DB_TYPES_H

#define SG_KEYS 32
#define BLK_SIZE 512
#define key__t int
#define val__t int
typedef unsigned long ptr__t;

struct MaybeValue
{
    char found;
    val__t value;
};

struct Query
{
    /* everything is a long to make debugging in gdb easier */
    key__t key;
    long found;
    long state_flags;

    val__t value;

    /* Current node - information */
    int current_iteration;
};

struct ScatterGatherQuery
{
    ptr__t value_ptr;

    int index;
    int n_keys;
    key__t keys[SG_KEYS];
    struct MaybeValue values[SG_KEYS];

    unsigned int state_flags;
    // use for saving the results
    int current_index;
    // use for searching

    int current_array_index;
    int current_iteration;
    ptr__t array;
    int iteration;
};

static inline struct Query new_query(long key)
{
    struct Query query =
        {
            .key = key,
            .found = 0,
            .state_flags = 0,
            .value = 0,
            .current_iteration = 0,
        };
    return query;
}

#endif /* DB_TYPES_H */