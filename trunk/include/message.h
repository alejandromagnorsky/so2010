/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       message.h
**  Content:    structures and functions related to the message_t ADT.
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

message_t   mnew    (int from, int to, size_t len, char* data);
/* Creates a new message, given the parameters. */

message_t   mcopy   (message_t);
/* Returns a deep copy of an existing message. */

void        mdel    (message_t);
/* Frees the memory allocated for a message. */

int         mfrom   (message_t);
/* Returns the 'from' integer passed on message creation. */

int         mto     (message_t);
/* Returns the 'to' integer passed on message creation. */

int         mdlen   (message_t);
/* Returns the 'len' integer, the data string length in bytes. */

char*       mdata   (message_t);
/* Returns the data string of the message. */

int         mfsize  (message_t);
/* Returns the full size of the message (the header structure and the data). */

int         mcmp    (message_t, message_t);
/* Returns 1 if the messages contain the same information (to, from, data), and
   0 otherwise. */

void        mprintln(message_t);
/* Prints the full message (to, from, data) to stdout, plus a newline. */
#endif
