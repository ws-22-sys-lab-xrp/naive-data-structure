#include <stdio.h>
#include <sys/fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

#include "helpers.h"

#define LOAD_MODE 0
#define RUN_MODE 1

// TODO: create easylist
// Store in the disk

int get_handler(char *store_path, int flag)
{
    // Mark to use it on XRP
    // int fd = open(store_path, flag | O_DIRECT, 0644);
    int fd = open(store_path, flag, 0644);
    if (fd < 0)
    {
        printf("Failed to open file %s!\n", store_path);
        exit(1);
    }
    return fd;
}

int initialize(size_t num, int mode, char *store_path)
{
    int store;
    if (mode == LOAD_MODE)
    {
        store = get_handler(store_path, O_CREAT | O_TRUNC | O_WRONLY);
    }
    else
    {
        store = get_handler(store_path, O_RDONLY);
    }

    printf("In total %lu elements (Current is integers)\n", num);
    return store;
}

/* Create a new store on disk at [store_path] with [elements]*/
int load(size_t num, char *store_path)
{
    int store_fd = initialize(num, LOAD_MODE, store_path);

    //  Seed the random number generator with the current time
    srand(time(NULL));

    int array[num];
    for (int i = 0; i < num; i++)
    {
        array[i] = rand() % num;
    }

    //    TODO:store in the store_path
    ssize_t write_size = num * sizeof(int);
    ssize_t bytes_writen = write(store_fd, array, write_size);

    printf("write_size: %lu\n", write_size);
    printf("bytes_writen: %lu\n", bytes_writen);

    if (write_size < 0)
    {
        fprintf(stderr, "failure: partial write of index node\n");
        exit(1);
    }

    close(store_fd);
    return 0;
}

int fetch(int fd, int index)
{
    char buffer[sizeof(int)];
    pread(fd, buffer, sizeof(int), index * sizeof(int));
    return *((int *)buffer);

    // int value = *((int *)buffer);
    // printf("Value at index %i: %d\n", index, value);
    // return value;
}

int operate(char *store_path, int pass_in_index, int num, int iteration)
{
    int fd = open(store_path, O_RDONLY);
    int index = pass_in_index;
    int value;
    for (int i = 0; i < iteration; i++)
    {
        value = fetch(fd, index);
        index = 2 * value % num;
    }
    return fetch(fd, index);
}

int process(char *store_path, int pass_in_index, int num, int iteration)
{
    // TODO: need to cancel use xrp
    int use_xrp = 1;

    int bpf_fd = -1;
    if (use_xrp)
    {
        bpf_fd = load_bpf_program("xrp-bpf/operate.o");
    }

    int flags = O_RDONLY;
    if (use_xrp)
    {
        flags = flags | O_DIRECT;
    }
    int db_fd = open(store_path, flags);
    struct Query query = new_query(pass_in_index);

    // normal processing
    printf("Normal Processing...\n");
    int result = operate("hanwen_db", 0, num, iteration);
    printf("Normal Process\n Result is : %d\n", result);

    if (use_xrp)
    {
        printf("XRP Processing...\n");
        lookup_bpf(db_fd, bpf_fd, &query, 0);
        int xrp_result = query.value;
        printf("XRP Process\n Result is : %d\n", xrp_result);
    }
}

int main()
{
    int num = 100;
    int iteration = 10000;
    clock_t generate_start = clock();
    load(num, "hanwen_db");
    clock_t generate_end = clock();
    printf("Generating Time is %f seconds.\n", (double)(generate_end - generate_start) / CLOCKS_PER_SEC);

    // int fd = open("hanwen_db", O_RDONLY);
    // for (int i = 0; i < 1000; i++)
    // {
    //     fetch(fd, i);
    // }
    clock_t compute_start = clock();
    int result = operate("hanwen_db", 0, num, iteration);
    clock_t compute_end = clock();
    printf("Ground Truth Result is : %d\n", result);
    printf("Ground Truth Computing Time is %f seconds.\n", (double)(compute_end - compute_start) / CLOCKS_PER_SEC);

    /* Above is for testing usage*/
    printf("Currently in the comparison\n\n");
    process("hanwen_db", 0, num, iteration);

    return 0;
}
