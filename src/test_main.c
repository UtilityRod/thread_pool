#include <test_main.h>

static int compare(const void * arg1, const void * arg2)
{
    (void)arg1;
    (void)arg2;
    return 0;
}

static void destroy(void * arg)
{
    (void)arg;
    return;
}

static void * pthread_function(void * arg)
{
    int * int_arg = (int *)arg;
    printf("Hello from thread %d\n", *int_arg);
    return NULL;
}

int main(void)
{
    printf("Hello from test main.\n");
    thread_pool_t * p_pool = pool_create(5, compare, destroy, pthread_function);

    int array[100];

    for(int i = 1; i <= 100; i++)
    {
        array[i - 1] = i;
        pool_add_work(p_pool, &(array[i-1]));
    }

    sleep(5);
    pool_destroy(p_pool);
}