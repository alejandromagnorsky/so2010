/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       queue.c
**  Content:    structures and functions related to the queue_t ADT.
*/

#include "../include/queue.h"

/* [TODO] IMPLEMENT LOCKS! NOT THREAD-SAFE AS OF NOW! */

int queueCount() {
    /* This function will generate unique queue ids within each process */
    static int n = 0;
    return n++;
}

queue_t newQueue() {
    queue_t ret = (queue_t) malloc(sizeof(struct st_queue_t));
    
    pthread_mutex_init(&(ret->lock), NULL);
    ret->first = ret->last = NULL;
    
    return ret;
}

void lockQueue(queue_t queue) {
    pthread_mutex_lock(&(queue->lock));
}

void unlockQueue(queue_t queue) {
    pthread_mutex_unlock(&(queue->lock));
}

message_t qGet(queue_t queue) {

    item_t i;
    message_t ret = NULL;

    lockQueue(queue);

    if ((i = queue->first) != NULL) {
    
        ret = i->message;
        queue->first = i->next;
        free(i);
    }
    
    unlockQueue(queue);
    
    return ret;
}

int qPut(queue_t queue, message_t message) {
    
    item_t i = (item_t) malloc(sizeof(struct st_item_t));
    
    if (!i)
        return 0; /* returns false immediately (failure) */
    
    i->message = copyMessage(message);
    i->next = NULL;
    
    lockQueue(queue);
    
    if (queue->first == NULL) {
        queue->first = queue->last = i;
        
    } else {
        queue->last->next = i;
        queue->last = i;
    }
    
    unlockQueue(queue);
    
    return 1; /* return true (success) */
}

void delQueue(queue_t queue) {

    item_t this, next;
    pthread_mutex_t* lockaddr;
    
    lockQueue(queue);
    
    lockaddr = &(queue->lock);
    next = queue->first;
    
    while((this = next) != NULL) {
        next = this->next;
        
        delMessage(this->message);
        free(this);
    }
    
    free(queue);
    pthread_mutex_destroy(lockaddr);
    
}
