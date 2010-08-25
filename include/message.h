/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       message.h
**  Content:    structures and functions related to the message_t ADT.
*/

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "tools.h"

struct st_message_t {

    size_t  len;
    char*   data;
    
};

typedef struct st_message_t* message_t;

/* This macro clarifies the serialized message transfer protocol
   DO NOT CONFUSE THIS WITH sizeof(struct st_message_t) */

#define M_HEADER_SIZE (sizeof(size_t))

message_t   mnew    (size_t len, char* data);
/* Creates a new message, deep-copying the data. */

message_t   mcopy   (message_t);
/* Returns a deep copy of an existing message. */

void        mdel    (message_t);
/* Frees the memory allocated for a message. */

int         mdlen   (message_t);
/* Returns the 'len' variable, the data string length in bytes. */

char*       mdata   (message_t);
/* Returns the data string address of the message. */

int         minsert (message_t, char* buf, size_t start, size_t n);
/* Copies n bytes from buf to (message data + start). Returns the number of
   bytes copied (which can be <n if minsert were to exceed the allocated len) */

int         mfsize  (message_t);
/* Returns the full size of the message (the structure size + data length). */

int         mcmp    (message_t, message_t);
/* Returns 1 if the messages contain the same information, and 0 otherwise. */
   
char*       mserial     (message_t);
/* Serializes a message. Allocates the necessary memory and returns a string. */

message_t   mdeserial   (char*);
/* Extracts a message from a given serialized string. */

void        mprintln(message_t);
/* Prints the full message to stdout, plus a newline. */
    
#endif
