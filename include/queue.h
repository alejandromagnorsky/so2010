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

queue_t     newQueue();
void        lockQueue(queue_t);
void        unlockQueue(queue_t);
message_t   qGet(queue_t);
int         qPut(queue_t, message_t);
void        delQueue(queue_t);

#endif
