/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       tools.h
**  Content:    general purpose functions and declarations.
*/

#ifndef __TOOLS_H__
#define __TOOLS_H__

#include <string.h>
#include <stdio.h>

void* dmemcpy(void* dest, const void* src, size_t n);
/* Exactly like memcpy, only it returns a pointer to (dest + n).
   Helps clear the syntax in serialization methods (e.g. see mserial). */
   
void* smemcpy(void* dest, void* src, size_t n);
/* Exactly like memcpy, only it returns a pointer to (src + n).
   Helps clear the syntax in deserialization methods (e.g. see mdeserial). */


//#define LOGGING_ENABLED

#ifdef LOGGING_ENABLED
    #define LOG(...) fprintf(stderr, __VA_ARGS__)
    #define LOGPID(...) (fprintf(stderr, "(%d) ", getpid()), \
                         fprintf(stderr, __VA_ARGS__))

#else
    #define LOG(...)
    #define LOGPID(...)
#endif

#endif
