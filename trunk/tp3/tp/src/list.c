/*
** Doubly linked list implementation.
*/

/* struct ListNamespace {
    struct {
        lnode_t (*new) (void*);
        lnode_t (*free) (lnode_t);
    } Node;

    list_t  (*new)        ();
    lnode_t (*add)        (list_t, void*);
    lnode_t (*head)       (list_t);
    lnode_t (*tail)       (list_t);
    lnode_t (*get)        (list_t, unsigned int index);
    void    (*remove)     (list_t, unsigned int index);
    unsigned int (*count) (list_t);
}; */

#include "../include/list.h"
#include <stdlib.h>

lnode_t _node_new (void* data) {
    /* Using glibc's malloc. Obviously, this has to go. */
    lnode_t new = (lnode_t) malloc(sizeof(struct lnode_t));
    new->data = data;
    new->prev = new->next = NULL;
    
    return new;
}

list_t _list_new () {
    /* Using glibc's malloc. Obviously, this has to go. */
    list_t new = (list_t) malloc(sizeof(struct list_t));
    new->head = new->tail = (void*) (new->count = 0);
    
    return new;
}
    
lnode_t _list_add (list_t list, void* data) {

    lnode_t current, new;
    
    /* Are there any elements already in the list? */
    if ((current = list->head) != NULL) {
    
        while (current->next != NULL)
            current = current->next;
        
        /* Current now points to the last node in the list. */
        current->next = (new = _node_new(data));
        new->prev = current;
        
        list->tail = new;
        list->count++;
        
    } else { /* The list is empty. */
        list->head = list->tail = _node_new(data);
        list->count = 1;
    }
}

lnode_t _list_get (list_t, unsigned int index) {

}

void    _list_remove     (list_t, unsigned int index);
unsigned int _list_count (list_t);

struct ListNamespace List = { { _node_new,
                                NULL }, 
                               _list_new,
                               _list_add };
