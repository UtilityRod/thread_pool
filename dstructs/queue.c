#include <queue.h>
#include <circular_ll.h>
#include <stdio.h>
#include <stdlib.h>

struct queue_ {
    circular_list_t * p_list;
    size_t size;
};

queue_t * queue_create(compare_f compare, destroy_f destroy)
{
    queue_t * p_queue = calloc(sizeof(*p_queue), 1);

    if (p_queue == NULL)
    {
        perror("Queue memeory allocation.");
    }
    else
    {
        p_queue->p_list = circular_create(compare, destroy);
    }

    return p_queue;
}

void queue_destroy(queue_t * p_queue)
{
    if (p_queue != NULL && p_queue->p_list)
    {

        if (p_queue->p_list)
        {
           circular_destroy(p_queue->p_list); 
        }
        
        free(p_queue);
    }
}

size_t queue_enqueue(queue_t * p_queue, void * p_data)
{

    if (p_queue && p_queue->p_list)
    {
        p_queue->size = circular_insert(p_queue->p_list, p_data, true);
    }

    return p_queue->size;
}

void * queue_dequeue(queue_t * p_queue)
{
    void * p_data = NULL;

    if (p_queue->p_list)
    {
        p_data = circular_remove_at(p_queue->p_list, BACK);
    }

    return p_data;
}
// END OF SOURCE
