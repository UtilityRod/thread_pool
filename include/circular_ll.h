#ifndef _CIRCULAR_LL_H_
#define _CIRCULAR_LL_H_

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

typedef void (*destroy_f)(void * arg);
typedef int (*compare_f)(const void * arg1, const void * arg2);

// Circular Linked List
typedef struct circular_list_ circular_list_t;
typedef enum location_ {FRONT = -1, BACK = -2}location_t;
circular_list_t * circular_create(compare_f compare, destroy_f destroy);
void circular_destroy(circular_list_t * p_list);
size_t circular_insert(circular_list_t * p_list, void * p_data, bool add_front);
void * circular_search(circular_list_t * p_list, void * p_data);
size_t circular_delete(circular_list_t * p_list, void * p_data);
void * circular_remove_at(circular_list_t * p_list, location_t location);
void circular_ll_sort(circular_list_t * p_list);
#endif