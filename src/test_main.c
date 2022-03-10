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
    printf("Inside of pthread_function: %d\n", *int_arg);
    return NULL;
}

int main(void)
{
    printf("Hello from test main.\n");
    thread_pool_t * p_pool = pool_create(5, compare, destroy, pthread_function);
    int one = 1;
    int two = 2;
    int three = 3;
    int four = 4;
    int five = 5;
    sleep(3);
    printf("Adding one to pool\n");
    pool_add_work(p_pool, &one);
    sleep(3);
    printf("Adding two to pool\n");
    pool_add_work(p_pool, &two);
    printf("Adding three to pool\n");
    pool_add_work(p_pool, &three);
    sleep(3);
    printf("Adding four to pool\n");
    pool_add_work(p_pool, &four);
    sleep(3);
    printf("Adding five to pool\n");
    pool_add_work(p_pool, &five);
    sleep(5);
    pool_destroy(p_pool);
}