#include <stdlib.h>
#include <string.h>

#include "parse.h"
#include "helpers.h"

/* Parsing for main */

/* Parsing for DB creation */
static struct argp_option create_opts[] = {
    {0}};
static char create_doc[] = "Create a new database with the specified number of elements.";

static int _parse_create_opts(int key, char *arg, struct argp_state *state)
{
    if (key == CACHE_ARG_KEY)
    {
        argp_error(state, "unsupported argument");
    }
    return 0;
}

void parse_create_opts(int argc, char *argv[])
{
    struct argp argp = {create_opts, _parse_create_opts, "", create_doc};
    argp_parse(&argp, argc, argv, 0, 0, create_opts);
}

/* Parsing for get key benchmark */

/* Parsing for range query benchmark */
