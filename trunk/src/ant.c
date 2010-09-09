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
    
    ant->state = ANT_STATE_IDLE;
    
    return NULL;
}

cmd_t andHandleTurn(void* antarg, cmd_t cmd) {
    int i, mindir;
    tile_t tile;
    ant_t ant = (ant_t) antarg;
    
    switch (ant->state) {
    
        case ANT_STATE_SEEKING:
        
            if (ant->smelled) {
                /* Look up, right, down, left: */
                mindir = -1;
                for(i = 0; i < 4; i += 2) {
                    mindir = ant->smell[i].trail > ant->smell[i].trail ?
                             ant->smell[i].trail : ant->smell[i].trail;
                }
                    
            } else {
                return newSmellReq();

    
    }
    return NULL;

}

void antFillHandlerArray(handler_f* handlers) {
    handlers[CMD_START] = antHandleStart;
}
