#include <thread_pool.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
struct thread_pool_ {
    uint32_t num_threads; // The total amount of threads to spawn
    queue_t * p_queue;
    pthread_t * p_threads;
    pthread_mutex_t lock;
    pthread_cond_t conditional_var;
    pthread_f function;
};

static void * thread_private(void * p_arg);

thread_pool_t * pool_create(uint32_t num_threads, pthread_f function)
{
    thread_pool_t * p_pool = calloc(sizeof(*p_pool), 1);

    if (p_pool == NULL)
    {
        perror("Thread pool memory allocation.");
    }
    else
    {
        p_pool->num_threads = num_threads;
        p_pool->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        p_pool->conditional_var = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
        p_pool->function = function;
        pthread_mutex_lock(&(p_pool->lock));
        p_pool->p_queue = queue_create(NULL, NULL);
        p_pool->p_threads = calloc(sizeof(pthread_t *), num_threads);

        if ((p_pool->p_threads == NULL) || (p_pool->p_queue == NULL))
        {
            perror("Threads array memory allocation.");
            pool_destroy(p_pool);
            free(p_pool);
            p_pool = NULL;
        }
        else
        {
            for(uint32_t i = 0; i < num_threads; i++)
            {
                pthread_create(&(p_pool->p_threads[i]), NULL, thread_private, p_pool);
            }
        }
        pthread_mutex_unlock(&(p_pool->lock));
    }

    return p_pool;
}

void pool_destroy(thread_pool_t * p_pool)
{
    if (p_pool != NULL)
    {
        if (p_pool->p_queue)
        {
            queue_destroy(p_pool->p_queue);
        }

        if (p_pool->p_threads)
        {
            for (uint32_t i = 0; i < p_pool->num_threads; i++)
            {
                pthread_join(p_pool->p_threads[i], NULL);
            }
            free(p_pool->p_threads);
        }

        free(p_pool);
    }
}

int pool_add_work(thread_pool_t * p_pool, void * p_work)
{
    int rv = 0;
    if(p_pool->p_queue != NULL)
    {
        pthread_mutex_lock(&(p_pool->lock));
        queue_enqueue(p_pool->p_queue, p_work);
        pthread_cond_signal(&(p_pool->conditional_var));
        pthread_mutex_unlock(&(p_pool->lock));
        rv = 1;
    }
    
    return rv;
}

static void * thread_private(void * p_arg)
{
    thread_pool_t * p_pool = (thread_pool_t *)p_arg;

    while (1)
    {
        pthread_mutex_lock(&(p_pool->lock));
        void * work = NULL;
        if ((work = queue_dequeue(p_pool->p_queue)) == NULL)
        {
            pthread_cond_wait(&(p_pool->conditional_var), &(p_pool->lock));
            work = queue_dequeue(p_pool->p_queue);
        }

        pthread_mutex_unlock(&(p_pool->lock));

        if (work != NULL)
        {
            p_pool->function(work);  
        }    
    }
    
    return NULL;
}