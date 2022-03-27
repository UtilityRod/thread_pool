#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <stdint.h>

typedef struct thread_job_ thread_job_t;
typedef struct thread_pool_ thread_pool_t;
typedef void * (*pthread_f) (void * p_arg);

thread_pool_t * pool_create(uint32_t num_threads, pthread_f function);
void pool_destroy(thread_pool_t * p_pool);
void pool_add_work(thread_pool_t * p_pool, void * p_work);
void pool_kill_all(thread_pool_t * p_pool);

#endif