/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       message.h
**  Content:    structures and functions common to all implemented IPC methods.
*/

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdlib.h>
#include <string.h>

struct st_message_t {
/*
**  The message_t type is an abstract data type intended to be immutable. Only
**  a creator, destroyer and copier are provided to that end.
*/
    int     from, to;
    int     len;
    char*   data;
    
};

typedef struct st_message_t* message_t;

message_t   newMessage  (int from, int to, size_t len, char* data);
message_t   copyMessage (message_t);
void        delMessage  (message_t);

char        cmpMessage  (message_t, message_t);

void        printMessage(message_t);

#endif
