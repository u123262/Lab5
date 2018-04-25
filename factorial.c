#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <pthread.h>
#include <stdbool.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int k = 0;
int mod = 0;
int tnum = 0;

int current_value = 1;
int f = 1;

void *factorial(void *args)
{
    while (current_value <= k)
    {
        pthread_mutex_lock(&mutex);
        f *= current_value;
        current_value += 1;
        pthread_mutex_unlock(&mutex);
    }
}

int main(int argc, char **argv)
{
    while (true) 
    {
            int current_optind = optind ? optind : 1;

            static struct option options[] = 
            {
                { "k", required_argument, 0, 0 },
                { "mod", required_argument, 0, 0 },
                { "tnum", required_argument, 0, 0 },
                { 0, 0, 0, 0 } 
            };

            int option_index = 0;
            int c = getopt_long(argc, argv, "f", options, &option_index);

            if (c == -1) break;

            switch (c) {
            case 0:
                switch (option_index) {
                case 0:
                    k = atoi(optarg);
                    break;
                case 1:
                    mod = atoi(optarg);
                    break;
                case 2:
                    tnum = atoi(optarg);
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

    pthread_t *threads = (pthread_t *)malloc(sizeof(pthread_t) * tnum);
    for (int i = 0; i < tnum; i++)
    {
        if (pthread_create(&threads[i], NULL, factorial, NULL) != 0)
        {
            perror("pthread_create");
            exit(1);
        }
    }
    for (int i = 0; i < tnum; i++)
    {
        pthread_join(threads[i], NULL);
    }
    free(threads);
    
    printf("Result = %d\n", f % mod);
    return 0;
}