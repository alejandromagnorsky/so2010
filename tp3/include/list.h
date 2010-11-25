/*
** Doubly linked list implementation.
*/

#ifndef __LIST_H__
#define __LIST_H__

struct lnode_t {
    void* data;
    struct lnode_t *prev, *next;
};

typedef struct lnode_t* lnode_t;
    
struct list_t {
    lnode_t head, tail;
    unsigned int count;
};

typedef struct list_t* list_t;

struct ListNamespace {
    struct {
        lnode_t (*new) (void*);
        lnode_t (*free) (lnode_t);
    } Node;

    list_t  (*new)        ();
    lnode_t (*add)        (list_t, void*);
    lnode_t (*get)        (list_t, unsigned int index);
    void    (*remove)     (list_t, unsigned int index);
    unsigned int (*count) (list_t);
};

lnode_t _list_add        (list_t, void*);
lnode_t _list_get        (list_t, unsigned int index);
void    _list_remove     (list_t, unsigned int index);
unsigned int _list_count (list_t);

#endif
