#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/file.h>
#include <sys/types.h>
#include <errno.h>
#include <time.h>
//#include <argp.h>

#include "easylist.h"
#include "helpers.h"
#include "parse.h"
#include "create.h"
#include "get.h"

int main(int argc, char *argv[])
{
//    printf("test 123");


    struct argp_option options[] = {{0}};
    struct ArgState arg_state = default_argstate();
    struct argp argp = {options, parse_opt, "DB_NAME N_LAYERS CMD [CMD_ARGS] [CMD_OPTS]", doc};
    argp_parse(&argp, argc, argv, ARGP_IN_ORDER, 0, &arg_state);

    return arg_state.subcommand_retval;
}