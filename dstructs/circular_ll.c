#include <circular_ll.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct node_ {
    void * p_data;
    struct node_ * p_next;
    struct node_ * p_previous;
} node_t;

struct circular_list_ {
    node_t * p_head;
    size_t size;
    compare_f compare;
    destroy_f destroy;
    bool return_node;
};

circular_list_t * circular_create(compare_f compare, destroy_f destroy)
{
    circular_list_t * p_list = NULL;

    if (compare == NULL)
    {
        fputs("Compare function for linked list must not be NULL.\n", stderr);
    }
    else
    {
        p_list = calloc(sizeof(*p_list), 1);
        if (p_list == NULL)
        {
            // Memory allocation failed.
            perror("Linked list memory allocation.\n");
        }
        else
        {
            p_list->compare = compare; // Function to compare node data
            p_list->destroy = destroy; // Function to destroy node data if needed
        }
    }
    
    return p_list;
}

void circular_destroy(circular_list_t * p_list)
{
    if (p_list->p_head != NULL)
    {
        // If there are nodes inside of the list, delete all the nodes
        node_t * p_current = p_list->p_head;
        do
        {
            node_t * p_delete = p_current; // Save current node off for delete later
            p_current = p_current->p_next; // Move to next node in list

            if (p_list->destroy != NULL)
            {
                // Destroy function registered, call on each node's data to free all memory
                p_list->destroy(p_delete->p_data);
            }

            free(p_delete);
        } while (p_current != p_list->p_head);
    }

    p_list->p_head = NULL;
    free(p_list);
}

size_t circular_insert(circular_list_t * p_list, void * p_data, location_t location)
{
    if (p_list != NULL && p_data != NULL)
    {
        node_t * p_new = calloc(sizeof(*p_new), 1);
        switch (p_new == NULL)
        {
            case 0: ; // Memory allocation for new node did not fail
                p_new->p_data = p_data; // Assign data to new node's data pointer variable
                if (p_list->p_head == NULL)
                {
                    // List is empty
                    p_list->p_head = p_new; // I am the head of the list
                    p_new->p_next = p_new; // My next node is me
                    p_new->p_previous = p_new; // My previous node is me
                }
                else
                {
                    // List is not empty
                    // The head node's previous node pointer is the last node in the list
                    node_t * p_last = p_list->p_head->p_previous;
                    // Update the next pointer of the last node in the list to the new node pointer
                    p_last->p_next = p_new;
                    // Update the new node's previous pointer to the last element in the list
                    p_new->p_previous = p_last;
                    // Update the head node's previous pointer to the new node
                    p_list->p_head->p_previous = p_new;
                    // Update the new node's next point to the list's head node
                    p_new->p_next = p_list->p_head;
                }

                if (location == FRONT)
                {
                    // Adding node to the front
                    // Update list head pointer to new node
                    p_list->p_head = p_new;
                }

                p_list->size++;
                break;
            default:
                perror("New node memory allocation for linked list.");
                break;
        }  
    }

    return p_list->size;
}

void * circular_search(circular_list_t * p_list, void * p_data)
{
    node_t * p_search = NULL;
    if (p_list->p_head == NULL)
    {
        // Linked list is empty
        fputs("Circular Linked List is Empty\n", stderr);
    }
    else
    {
        // Iterate through list to find node containing the data passed to function
        node_t * p_current = p_list->p_head;

        do
        {
            if (p_list->compare(p_data, p_current->p_data) == 0)
            {
                // Found the node containing the data
                p_search = p_current;
                break;
            }
            p_current = p_current->p_next; // Move to next node in the list
        } while (p_current != p_list->p_head);
    }

    void * return_data = NULL;
    if (p_search != NULL)
    {
        // The search node was found. Return either the data or the node pointer
        if (p_list->return_node)
        {
            // Return node flag invoked, return the pointer to the node
            return_data = p_search;
        }
        else
        {
            // Return node flag not invoked, return the node's data
            return_data = p_search->p_data;
        }
    }

    return return_data;
}

size_t circular_delete(circular_list_t * p_list, void * p_data)
{
    p_list->return_node = true;
    node_t * p_delete = circular_search(p_list, p_data);
    p_list->return_node = false;

    if (p_delete != NULL)
    {
        // Data found in linked list, figure out where in list it resides and update pointers
        switch (p_list->size)
        {
            case 1: ;
                // Node to delete is the head node, and there is the only node in the list
                // Set list head node to NULL
                p_list->p_head = NULL;
                break;
            default: // List has more than one node inside of it.
                p_delete->p_previous->p_next = p_delete->p_next; // Update delete node's previous node's pointer
                p_delete->p_next->p_previous = p_delete->p_previous; // Update delete node's next node's pointer
                if (p_delete == p_list->p_head)
                {
                    // The node to delete is the head node, new head node is delete node's next node
                    p_list->p_head = p_delete->p_next;
                }
        }

        if (p_list->destroy)
        {
            // If destroy function was registered call with delete node's data
            p_list->destroy(p_delete->p_data);
        }
        free(p_delete);
        p_list->size--;
    }

    return p_list->size;
}

void * circular_remove_at(circular_list_t * p_list, location_t location)
{
    node_t * p_remove = NULL;
    if ((p_list != NULL) && (p_list->p_head != NULL) && !((location > (int)p_list->size) || (location < BACK)))
    {
        /*
         * Condition 1: List must not be NULL
         * Condition 2: List must have at least one node
         * Condition 3: Location (x) must be BACK (-2) <= x < list size 
         */ 
        switch(location)
        {
            case FRONT: ;
                // Remove node is the first node in the list
                p_remove = p_list->p_head;
                break;
            case BACK: ;
                // Remove node is the last node in the list
                // Head node's previous node
                p_remove = p_list->p_head->p_previous;
                break;
            default:
                // Any other number will get the correct index in the list
                p_remove = p_list->p_head;
                for (int i = 1; i < location; i++)
                {
                    // Iterate through list until correct index
                    p_remove = p_remove->p_next;
                }
                break;
        }
    }

    void * return_data = NULL;
    if (p_remove != NULL)
    {
        // Remove node found
        // Update pointers for node's previous and next nodes;
        p_remove->p_next->p_previous = p_remove->p_previous;
        p_remove->p_previous->p_next = p_remove->p_next;

        switch (p_list->size)
        {
            case 1: ;// Only one element in the list zero out pointer
                p_list->p_head = NULL;
                break;
            default: // More than one node in the list
                if (p_remove == p_list->p_head)
                {
                    // Removing head node, new head for list is remove node's next node
                    p_list->p_head = p_remove->p_next;
                }
                break;
        }

        

        return_data = p_remove->p_data;
        p_list->size--;
        free(p_remove);
    }

    return return_data;
}

void circular_ll_sort(circular_list_t * p_list)
{
    if (p_list->p_head == NULL)
    {
        fputs("Circular Linked List is Empty\n", stderr);
    }
    else
    {
        void ** data_array = calloc(sizeof(*data_array), p_list->size); // Create temporary array of void * to hold every node's data
        size_t size = p_list->size; // Temporary variable containing the original size of the linked list
        for (size_t i = 0; i < size; i++)
        {
            // Insert each node's data into the array to be sorted
            data_array[i] = circular_remove_at(p_list, FRONT);
        }
        // Sort the array with qsort and the compare function
        qsort(data_array, size, sizeof(void *), p_list->compare);
        for (size_t i = 0; i < size; i++)
        {
            // Insert each node's data into the array to be sorted
            circular_insert(p_list, data_array[i], BACK);
        }
        free(data_array);
    }
}
// END OF SOURCE
