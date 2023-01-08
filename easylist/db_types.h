#define SG_KEYS 32
#define key__t int
#define val__t int

struct MaybeValue
{
    char found;
    val__t value;
};

struct ScatterGatherQuery
{
    int current_index;
    int n_keys;
    key__t keys[SG_KEYS];
    struct MaybeValue values[SG_KEYS];
};