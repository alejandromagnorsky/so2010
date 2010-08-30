/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       message.c
**  Content:    structures and functions related to the message_t ADT.
*/

#include "../include/message.h"

message_t mnew(int from, int to, size_t len, char* data) {

    message_t new = (message_t) malloc(sizeof(struct st_message_t) + len);
    
    if (new) {
        new->header.from = from;
        new->header.to = to;
        new->header.len = len;
        
        /*  The following pointer arithmetic avoids a second call to malloc */
        new->data = (char*) (new + 1);
        
        if (data != NULL)
            strncpy(new->data, data, len);
        else
            memset(new->data, 'X', len);

        return new;
        
    } else
        return NULL;
}

message_t mhnew(mheader_t h, char* data) {
    return mnew(h->from, h->to, h->len, data);
}

message_t mcopy(message_t m) {
    return mnew(m->header.from, m->header.to, m->header.len, m->data);
}

void mdel(message_t m) {
    free(m);
}

int     mfrom(message_t m)  { return m->header.from; }
int     mto(message_t m)    { return m->header.to;   }
int     mdlen(message_t m)  { return m->header.len;  }
char*   mdata(message_t m)  { return m->data;        }

int minsert (message_t m, char* buf, size_t start, size_t n) {
/*    int end, len;
    
    len = mdlen(m);
    
    if (start < len) {
    
        n = (start + n <= len) ? n : len - n;
        
        memcpy(buf, m->data,
        
    } else
        return 0;
*/
}

int mfsize(message_t m) {
    return (sizeof(struct st_message_t) + m->header.len);
}

int mcmp(message_t m1, message_t m2) {

    return (memcmp(&(m1->header), &(m2->header), M_HEADER_SIZE) == 0 &&
            strncmp(m1->data, m2->data, m1->header.len) == 0);

}

char* mserial(message_t m) {

    void* pos;
    char* ret;    
    
    pos = ret = (char*) malloc(M_HEADER_SIZE + m->header.len);
    
    pos = dmemcpy(pos, &(m->header), M_HEADER_SIZE);
    pos = dmemcpy(pos, m->data, m->header.len);
    
    return ret;

}

message_t mdeserial(char* data) {

    void* pos;
    struct st_mheader_t h;
    
    pos = smemcpy(&h, data, M_HEADER_SIZE);
    return mhnew(&h, pos);

}

void mprintln(message_t m) {

    printf("<Message %d-%d (%db): \"%*s\">\n", m->header.from,
                                               m->header.to,
                                               m->header.len,
                                               (int) m->header.len, m->data);

}
