/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       message.c
**  Content:    structures and functions related to the message_t ADT.
*/

#include "../include/message.h"

message_t mnew(size_t len, char* data) {

    message_t new = (message_t) malloc(sizeof(struct st_message_t) + len);
    
    if (new) {
        new->len = len;
        
        /*  The following pointer arithmetic avoids a second call to malloc */
        new->data = (char*) (new + 1);
        strncpy(new->data, data, len);

        return new;
        
    } else
        return NULL;
}

message_t mcopy(message_t m) {
    return mnew(m->len, m->data);
}

void mdel(message_t m) {
    free(m);
}

int     mdlen(message_t m)  { return m->len;    }
char*   mdata(message_t m)  { return m->data;   }

int minsert (message_t m, char* buf, size_t start, size_t n) {
    int end;
    
    if (start < m->len) {
    
        n = (start + n <= m->len) ? n : m->len - n;
        
        memcpy(buf, m->data,
        
    } else
        return 0;

}

int mfsize(message_t m) {
    return (sizeof(struct st_message_t) + m->len);
}

int mcmp(message_t m1, message_t m2) {

    return (m1->len == m2->len &&
            strncmp(m1->data, m2->data, m1->len) == 0);

}

char* mserial(message_t m) {

    void* pos;
    char* ret;
    
    pos = ret = (char*) malloc(sizeof(struct st_message_t) + m->len);
    
    pos = dmemcpy(pos, &(m->len),  sizeof(m->len));
    pos = dmemcpy(pos, m->data, m->len);
    
    return ret;

}

message_t mdeserial(char* data) {

    void* pos;
    size_t len;
    
    pos = smemcpy(&len, data, sizeof(len));

    return mnew(len, pos);

}

void mprintln(message_t m) {

    printf("<Message: \"%*s\">\n", 
           m->len, m->data);

}
