/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       list.h
**  Content:    structures and functions for a very, very, VERY basic linked
**              list implementation.
*/

typedef struct node_t* node_t;

struct node_t {
    void* data;
    node_t next;
}

node_t nnew(void* data);
void   nadd(node_t, node_t);
node_t nnext(node_t);
