#include "thread_pool.h"
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <signal.h>
#include <queue.h>

struct thread_pool_ {
    uint32_t num_threads; // The total amount of threads to spawn
    queue_t * p_queue; // Queue that works get added to
    pthread_t * p_threads; // Array that tracks threads
    pthread_mutex_t lock; // Mutex lock to protect critical code
    pthread_cond_t conditional_var; // Conditional variable to protect critical code
    pthread_f function; // The working function a thread is to perform
};

static void * thread_private(void * p_arg);

volatile sig_atomic_t exit_flag = 0;

thread_pool_t * pool_create(uint32_t num_threads, pthread_f function)
{
    thread_pool_t * p_pool = calloc(sizeof(*p_pool), 1);

    if (p_pool == NULL)
    {
        // Memory allocation of pool failed
        perror("Thread pool memory allocation.");
    }
    else
    {
        // Assign all correct variables to struct
        p_pool->num_threads = num_threads;
        p_pool->lock = (pthread_mutex_t)PTHREAD_MUTEX_INITIALIZER;
        p_pool->conditional_var = (pthread_cond_t)PTHREAD_COND_INITIALIZER;
        p_pool->function = function;
        // Lock the mutex to stop race condition for threads being spawned
        p_pool->p_queue = queue_create(NULL, NULL);
        p_pool->p_threads = calloc(sizeof(pthread_t *), num_threads);

        if ((p_pool->p_threads == NULL) || (p_pool->p_queue == NULL))
        {
            // Either the threads array or the queue was not properly allocated
            perror("Threads array memory allocation.");
            pool_destroy(p_pool); // Call destroy to properly free all memory
            p_pool = NULL; // NULL out pointer to struct
        }
        else if (p_pool->function == NULL)
        {
            // You have to provide a function to the pool in order to perform work
            fputs("Function for threads must not be NULL", stderr);
            pool_destroy(p_pool); // Call destroy to properly free all memory
            p_pool = NULL; // NULL out pointer to struct
        }
        else
        {
            // Memory allocaiton succeeded, spawn N amount of threads specified by variable num_threads
            for(uint32_t i = 0; i < num_threads; i++)
            {
                pthread_create(&(p_pool->p_threads[i]), NULL, thread_private, p_pool);
            }
        }
    }

    return p_pool;
}

void pool_destroy(thread_pool_t * p_pool)
{
    if (p_pool != NULL)
    {
        // If the thread array exists, destroy all threads
        if (p_pool->p_threads != NULL)
        {
            for (uint32_t i = 0; i < p_pool->num_threads; i++)
            {
                pthread_join(p_pool->p_threads[i], NULL);
            }

            free(p_pool->p_threads);
        }
        // If the queue exists destroy it
        if (p_pool->p_queue)
        {
            queue_destroy(p_pool->p_queue);
        }

        free(p_pool);
        p_pool = NULL;
    }
}

void pool_add_work(thread_pool_t * p_pool, void * p_work)
{
    if(p_pool->p_queue != NULL)
    {
        pthread_mutex_lock(&(p_pool->lock)); // Lock access to the queue
        queue_enqueue(p_pool->p_queue, p_work); // Add the work to the queue
        pthread_cond_signal(&(p_pool->conditional_var)); // Send signal to threads that work is available
        pthread_mutex_unlock(&(p_pool->lock)); // Unlock access to the queue
    }
}

void pool_kill_all(thread_pool_t * p_pool)
{
    exit_flag = 1;
    pthread_cond_broadcast(&(p_pool->conditional_var)); // Broadcast to all threads to get passed conditional var
}

static void * thread_private(void * p_arg)
{
    // Argument passed to private thread funciton is the pool struct
    thread_pool_t * p_pool = (thread_pool_t *)p_arg;

    for(;;)
    {
        pthread_mutex_lock(&(p_pool->lock)); // Lock mutex due to use of conditional variable
        // Try to dequeue work off the queue
        while (queue_get_size(p_pool->p_queue) == 0) 
        {
            if (exit_flag != 0)
            {
                pthread_mutex_unlock(&(p_pool->lock));
                return NULL;
            }
            // No work in the queue, so wait on the conditional variable
            pthread_cond_wait(&(p_pool->conditional_var), &(p_pool->lock));
        }

        void * work = queue_dequeue(p_pool->p_queue);
        // Unlock access to the queue
        pthread_mutex_unlock(&(p_pool->lock));

        // Check to see if thread woke up without any work available
        if (work != NULL)
        {
            // The thread recieved work from the queue, call function provided on creation
            p_pool->function(work);  
        }    
    }
    
    return NULL;
}
// END OF SOURCE