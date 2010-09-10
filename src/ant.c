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
    
    return ret;
}

void antFree(ant_t ant) {
    free(ant);
}

cmd_t antHandleStart(void* antarg, cmd_t cmd) {
    ant_t ant = (ant_t) antarg;
	LOGPID("Handling start command.\n");    
    ant->state = ANT_STATE_SEEKING;
    
    return NULL;
}

cmd_t antHandleTurn(void* antarg, cmd_t cmd) {
	ant_t ant = (ant_t) antarg;
	LOGPID("Handling turn command.\n");	
	return newMoveReq(randDir());

}

void antFillHandlerArray(handler_f* handlers) {
    
    handlers[CMD_START] = antHandleStart;
    handlers[CMD_TURN] = antHandleTurn;
    
    return;
}
