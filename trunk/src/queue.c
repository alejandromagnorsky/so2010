/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       queue.c
**  Content:    structures and functions related to the queue_t ADT.
*/

#include "../include/queue.h"

int _queueCount() {
    /* This function will generate unique queue ids within each process */
    static int n = 0;
    return n++;
}

queue_t qnew() {
    queue_t ret = (queue_t) malloc(sizeof(struct st_queue_t));
    
    pthread_mutex_init(&(ret->lock), NULL);
    ret->first = ret->last = NULL;
    
    return ret;
}

void qdel(queue_t queue) {

    item_t this, next;
    pthread_mutex_t* lockaddr;
    
    qlock(queue);
    
    lockaddr = &(queue->lock);
    next = queue->first;
    
    while((this = next) != NULL) {
        next = this->next;
        
        mdel(this->message);
        free(this);
    }
    
    free(queue);
    pthread_mutex_destroy(lockaddr);
    
}

void qlock(queue_t queue) {
    pthread_mutex_lock(&(queue->lock));
}

void qunlock(queue_t queue) {
    pthread_mutex_unlock(&(queue->lock));
}

message_t qget(queue_t queue) {

    item_t i;
    message_t ret = NULL;

    qlock(queue);

    if ((i = queue->first) != NULL) {
    
        ret = i->message;
        queue->first = i->next;
        free(i);
    }
    
    qunlock(queue);
    
    return ret;
}

message_t qpeek(queue_t queue) {

    item_t i;
    message_t ret = NULL;

    qlock(queue);

    if ((i = queue->first) != NULL) {
        ret = mcopy(i->message);
    }
    
    qunlock(queue);
    
    return ret;
}

int qput(queue_t queue, message_t message) {
    
    item_t i = (item_t) malloc(sizeof(struct st_item_t));
    
    if (!i)
        return 0; /* returns false immediately (failure) */
    
    i->message = mcopy(message);
    i->next = NULL;
    
    qlock(queue);
    
    if (queue->first == NULL) {
        queue->first = queue->last = i;
        
    } else {
        queue->last->next = i;
        queue->last = i;
    }
    
    qunlock(queue);
    
    return 1; /* return true (success) */
}
