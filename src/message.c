/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       message.c
**  Content:    structures and functions related to the message_t ADT.
*/

#include "../include/message.h"

#include <stdio.h>

message_t mnew(int from, int to, size_t len, char* data) {

    message_t new = (message_t) malloc(sizeof(struct st_message_t) + len);
    
    if (new) {
        
        new->from = from;
        new->to = to;
        new->len = len;
        
        /*  The following pointer arithmetic avoids a second call to malloc */
        new->data = (char*) (new + 1);
        strncpy(new->data, data, len);

        return new;
        
    } else
        return NULL;
}

message_t mcopy(message_t m) {
    return mnew(m->from, m->to, m->len, m->data);
}

void mdel(message_t m) {
    free(m);
}

int     mfrom(message_t m)  { return m->from;   }
int     mto(message_t m)    { return m->to;     }
int     mdlen(message_t m)  { return m->len;    }
char*   mdata(message_t m)  { return m->data;   }

int mfsize(message_t m) {
    return (sizeof(struct st_message_t) + m->len);
}

int mcmp(message_t m1, message_t m2) {

    return (m1->from == m2->from &&
            m1->to == m2->to &&
            m1->len == m2->len &&
            strncmp(m1->data, m2->data, m1->len) == 0);

}

void mprintln(message_t m) {

    printf("<Message from %d to %d: \"%*s\">\n", 
           m->from, m->to, m->len, m->data);

}
