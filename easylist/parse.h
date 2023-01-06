#ifndef _PARSE_H_
#define _PARSE_H_


#include <argp.h>

#define CACHE_ARG_KEY 1337
#define RANGE_SUM_KEY 9999

struct ArgState
{
    /* Required Args */
    char *filename;
    int layers;

    int subcommand_retval;
};

struct GetArgs
{
    long key;

    /* Flags */
    int key_set;
    int xrp;

    int threads;
    int requests;
    size_t cache_level;
    size_t database_layers;
};

static inline struct ArgState default_argstate(void)
{
    struct ArgState as = {0};
    return as;
}

struct Range
{
    unsigned long begin;
    unsigned long end;
};

int run_subcommand(struct argp_state *state, char *cmd_name, int (*subcommand)(int argc, char *argv[], struct ArgState *));

void parse_create_opts(int argc, char *argv[]);

#endif /* _PARSE_H_ */