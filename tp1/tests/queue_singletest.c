/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       queue_singletest.c
**  Content:    single threaded test for the queue library.
*/

#include <stdio.h>
#include "../include/message.h"
#include "../include/queue.h"

#define MSG "holaHOLAtestTEST"

int main(int argc, char** argv) {

    message_t m1, m2, m3;
    queue_t q;
    
    q = qnew();
    m1 = mnew(10, 20, sizeof(MSG), MSG);
    m2 = mnew(14, 20, sizeof(MSG), MSG);
    m3 = mnew(14, 28, sizeof(MSG), MSG);
       
    printf("Testing qGet on empty queue... ");
    
    if (qget(q) == NULL)
        printf("ok.\n");
        
    else {
        printf("Failed! qGet returned != NULL.\n");
        exit(1);
    }

    printf("qPutting a message...");
    if (qput(q, m1))
        printf("ok.\n");
        
    else {
        printf("Failed! qPut returned false!\n");
        exit(1);
    }

    printf("qGetting and comparing to original... ");
    if ( mcmp(m1, m2 = qget(q)) )
        printf("ok.\n");
        
    else {
        printf("Failed! Got a different message!\n");
        exit(1);
    }
    
    printf("qPutting, qGetting and comparting 3 messages... ");
    
    if( qput(q, m1) && qput(q, m2) && qput(q, m3)) {
     
        if (mcmp(qget(q), m1) &&
            mcmp(qget(q), m2) &&
            mcmp(qget(q), m3))
            printf("ok!\n");
        
        else
            printf("failed! Wrong order, or message corruption.\n");
        
        
    } else
        printf("qPut failed!\n");
    
    // [TODO] further test the queue's capabilities, and write a multi-threaded
    // test.
}
