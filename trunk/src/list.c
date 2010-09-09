/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       list.c
**  Content:    structures and functions for a very, very basic linked
**              list implementation.
*/

#include "../include/list.h"

node_t nnew(void* data) {
    node_t ret = (node_t) malloc(sizeof(struct node_t));
    
    ret->data = data;
    ret->next = NULL;
    
    return ret;
}

void nadd(node_t n1, node_t n2) {
    n2->next = n1->next;
    n1->next = n2;
    
    return;
}

node_t nnext(node_t node) {
    return node->next;
}
