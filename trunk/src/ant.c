/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       ant.c
**  Content:    structures and functions related to the ants in the simulation.
*/

#include "../include/ant.h"
#include <stdio.h>

ant_t antNew() {
    ant_t ret = (ant_t) calloc(1, sizeof(struct ant_t));
    ret->state = ANT_STATE_ZERO;
}

void antFree(ant_t ant) {
    free(ant);
}

cmd_t antHandleStart(ant_t ant, cmd_t cmd) {
    printf("hola");
}
