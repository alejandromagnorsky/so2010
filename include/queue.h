/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       queue.h
**  Content:    structures and functions related to the queue_t ADT.
*/

#ifndef __QUEUE_H__
#define __QUEUE_H__

#include <stdlib.h>
#include "message.h"

#include <pthread.h>

struct st_item_t {
    message_t message;
    struct st_item_t* next;
};

typedef struct st_item_t* item_t;

struct st_queue_t {
/*
** The queue_t abstract data type is meant to enqueue messages in a thread-safe
** manner.
*/
    pthread_mutex_t lock;
    item_t first;
    item_t last;
    
};

typedef struct st_queue_t* queue_t;

int _queueCount();

queue_t     qnew();
/* Creates a new thread-safe queue. */

void        qdel(queue_t);
/* Frees the memory allocated for the queue, and all of its items. */

void        qlock(queue_t);
/* Acquires the queue's internal mutex lock. Already called by qget/qput. */

void        qunlock(queue_t);
/* Releases the queue's internal mutex lock. */

message_t   qget(queue_t);
/* Retrieves an item for the queue, or blocks until able to. */

int         qput(queue_t, message_t);
/* Puts an item in the queue, or blocks until able to. Returns 1 on success, 0
   on failure (for example, if there's no memory available). */

#endif
