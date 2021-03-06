#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <stdint.h>
#include <stdbool.h>

struct MultArgs
{
    int end;
    int begin;
};

int factorial(const struct MultArgs *);
void *Thread_factorial(void *);

uint32_t k = 0;
uint32_t pnum = 0;
uint32_t mod = 0;
int main(int argc, char **argv)
{
    while (true) {
    int current_optind = optind ? optind : 1;

    static struct option options[] = {{"k", required_argument, 0, 0},
                                    {"pnum", required_argument, 0, 0},
                                    {"mod", required_argument, 0, 0},
                                    {0, 0, 0, 0}};

    int option_index = 0;
    int c = getopt_long(argc, argv, "", options, &option_index);

    if (c == -1) break;

    switch (c) {
        case 0:
            switch (option_index) {
                case 0:
                k = atoi(optarg);
                if (k <= 0)
                {
                    printf("k must be positive");
                    mod = -1;
                }
                break;
                case 1:
                pnum = atoi(optarg);
                if (pnum <= 0)
                {
                    printf("pnum must be positive");
                    pnum = -1;
                }
                break;
                case 2:
                mod = atoi(optarg);
                if (mod <= 0)
                {
                    printf("mod is positive number");
                    mod = -1;
                }
                break; 

                defalut:
                printf("Index %d is out of options\n", option_index);
            }
            break;
        case '?':
            break;

        default:
            printf("getopt returned character code 0%o?\n", c);
        }
    }

    if (optind < argc) {
        printf("Has at least one no option argument\n");
        return 1;
    }
    if (mod == -1 || pnum == -1 || k == -1) {
        printf("Usage: %s --mod \"num\" --pnum \"num\" --k \"num\" \n",
            argv[0]);
        return 1;
    }

    pthread_t threads[pnum];
    int n = k/pnum;
    struct MultArgs args[pnum];
    for (uint32_t i = 0; i < pnum; i++) {
        args[i].begin = i*n + 1; args[i].end = (i+1)*n;
        if (pthread_create(&threads[i], NULL, Thread_factorial, (void *)&args[i])) {
            printf("Error: pthread_create failed!\n");
            return 1;
        }
    }
    int total_mult = 1;
    for (uint32_t i = 0;i < pnum; i++)
    {
        int mult = 1;
        pthread_join(threads[i], (void **)&mult);
        total_mult = (total_mult * mult) % mod;
    }
    printf("Total: %d\n", total_mult);
    return 0;
}

int factorial(const struct MultArgs *args)
{
    int res = 1;
    for (int i=args->begin;i<=args->end;i++)
    {
        res = (res * i) % mod;
    }
    return res;
}
void *Thread_factorial(void *args)
{
    struct MultArgs *mult_args = (struct MultArgs *)args;
    return (void *)(size_t)factorial(mult_args);
}