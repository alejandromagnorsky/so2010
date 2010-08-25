/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       tools.c
**  Content:    general purpose functions and declarations.
*/

#include "../include/tools.h"

void* dmemcpy(void* dest, const void* src, size_t n) {

    return (memcpy(dest, src, n) + n);

}

void* smemcpy(void* dest, void* src, size_t n) {

    memcpy(dest, src, n);
    return (src + n);

}
