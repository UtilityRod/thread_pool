#include <test_main.h>

static void establish_handler(int sig, void (*func)(int));
static void signal_handler(int sig);

volatile sig_atomic_t accept_flag = 0;

static void * pthread_function(void * arg)
{
    int * int_arg = (int *)arg;
    (void)int_arg;
    printf("Hello from thread %d\n", *int_arg);
    return NULL;
}

int main(void)
{
    printf("Hello from test main.\n");
    thread_pool_t * p_pool = pool_create(5, pthread_function);

    establish_handler(SIGINT, signal_handler);

    int array[100];

    for(int i = 1; i <= 100; i++)
    {
        array[i - 1] = i;
        pool_add_work(p_pool, &(array[i-1]));
    }

    while (1)
    {
        pause();
        if (accept_flag != 0)
        {
            pool_kill_all(p_pool);
            break;
        }
    }
    
    pool_destroy(p_pool);
}

static void establish_handler(int sig, void (*func)(int))
{
    struct sigaction sa = { .sa_handler = func};
    // Call sigaction to set signal handler to signal
    if (sigaction(sig, &sa, NULL) == -1)
    {
        perror("Signal action failed.");
    }
}

static void signal_handler(int sig)
{
    /*
     * Performs a certain action depending on which signal was caught
     */
    switch (sig)
    {
        case SIGINT: ;
            // If CTL+C caught, set accept_flag for exit
            accept_flag = 1;
            break;
        default:
            printf("Unknown signal caught: %d\n", sig);
    }
    
}
