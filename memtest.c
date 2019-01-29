/*
 * memtest - a simple memory throughput tester
 * author: matth@mtingers.com
 * version: 1.0
 *
 * compile: cc -Wall -O2 memtest.c -o memtest
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>

#define MB 1048576
char *mem_pool = NULL;

void usage()
{
    printf("usage: memtest <mem size in MiB>\n");
    exit(1);
}

double diff_time(struct timeval *start, struct timeval *stop)
{
    double sec  = stop->tv_sec  - start->tv_sec;
    double usec = stop->tv_usec - start->tv_usec;
    double diff = sec + (usec/1000000);
    return diff;
}

void memtest_validate(size_t bytes)
{
    char c = rand();
    char b = rand();
    char *m = mem_pool;
    size_t i = 0, j = bytes - 1;
    double diff;
    struct timeval start, stop;

    gettimeofday(&start, NULL);
    while(j > 0 && i < bytes) {
        m[i] = c;
        m[j] = b;
        if(m[i] != c) {
            printf("Stored byte did not match written byte!\n");
        }
        if(m[j] != b) {
            printf("Stored byte did not match written byte!\n");
        }
        j--, i++;
    }
    gettimeofday(&stop, NULL);
    diff = diff_time(&start, &stop);
    printf("%7s %10.2fMB/s %10.2f seconds\n", "cmp-rw:", (bytes/MB) / diff, diff);

}

void memtest_left_right(size_t bytes)
{
    char c = rand();
    char b = rand();
    char *m = mem_pool;
    size_t i = 0, j = bytes - 1;
    double diff;
    struct timeval start, stop;

    /*for(i = 0; i < bytes; i++) {
        c = rand();
        m[i] = c;
    }
    */
    gettimeofday(&start, NULL);
    while(j > 0 && i < bytes) {
        m[i] = c;
        m[j] = b;
        j--, i++;
    }
    gettimeofday(&stop, NULL);
    diff = diff_time(&start, &stop);
    printf("%7s %10.2fMB/s %10.2f seconds\n", "l/r:", (bytes/MB) / diff, diff);
    

}

void memtest_rw(size_t bytes)
{
    char c = rand();
    char b = c;
    char *m = mem_pool;
    size_t i = 0;
    double diff;
    struct timeval start, stop;

    gettimeofday(&start, NULL);
    for(i = 0; i < bytes; i++) {
        //c = rand();
        m[i] = c;
    }
    gettimeofday(&stop, NULL);
    diff = diff_time(&start, &stop);
    printf("%7s %10.2fMB/s %10.2f seconds\n", "write:", (bytes/MB) / diff, diff);

    gettimeofday(&start, NULL);
    for(i = 0; i < bytes; i++) {
        //c = rand();
        m[i] = c;
    }
    gettimeofday(&stop, NULL);
    diff = diff_time(&start, &stop);
    printf("%7s %10.2fMB/s %10.2f seconds\n", "write:", (bytes/MB) / diff, diff);

    gettimeofday(&start, NULL);
    for(i = bytes-1; i > 0; i--) {
        //c = rand();
        if(b != m[i])
            fprintf(stderr, "memtest_rw: %c != %c\n", b, m[i]);
    }
    gettimeofday(&stop, NULL);
    diff = diff_time(&start, &stop);
    printf("%7s %10.2fMB/s %10.2f seconds\n", "read:", (bytes/MB) / diff, diff);

    gettimeofday(&start, NULL);
    for(i = bytes-1; i > 0; i--) {
        //c = rand();
        if(b != m[i])
            fprintf(stderr, "memtest_rw: %c != %c\n", b, m[i]);
    }
    gettimeofday(&stop, NULL);
    diff = diff_time(&start, &stop);
    printf("%7s %10.2fMB/s %10.2f seconds\n", "read:", (bytes/MB) / diff, diff);


}

void run_memtest(size_t bytes)
{
    printf("Starting memtest: size=%lu bytes (%4.2fMiB) (this runs forever)...\n", bytes, (double)bytes/MB);
    srand(time(NULL));

    mem_pool = malloc(bytes*sizeof(*mem_pool));
    if(!mem_pool) {
        perror("malloc");
        exit(1);
    }

    while(1) {
        memtest_rw(bytes);
        memtest_left_right(bytes);
        memtest_validate(bytes);
    }
}

int main(int argc, char **argv)
{
    int max_mem_in_bytes = 32 * MB;

    if(argc != 2)
        usage();

    max_mem_in_bytes = MB * atoi(argv[1]);
    if(max_mem_in_bytes < 1) {
        fprintf(stderr, "Invalid parameter: %s", argv[1]);
        exit(1);
    }
    run_memtest(max_mem_in_bytes);
    return 0;
}
