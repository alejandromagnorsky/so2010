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

int antLoop(ipc_t ipc) {

    char stop;
    handler_f* handlers;
    ant_t ant;
    cmd_t cmd;
    message_t message;

    ant = antNew();
    LOGPID("Starting ant logic loop.\n");

    fflush(stdout);

    handlers = buildHandlerArray();
    antFillHandlerArray(handlers);

    sendMessage(ipc, message = mnew(ipc->id, 1, sizeof(struct cmd_start_t),
                                    (char*) (cmd = newStart())));
    
    mdel(message);
    free(cmd);
    
    stop = 0;

    while(!stop) {

        if (message = recvMessage(ipc)) {
            LOGPID("Ant %d received: ", ipc->id);
            mprintln(message);
            dispatchCmd((void*) ant, (cmd_t) mdata(message), handlers);
            
        }
    
    }
}

cmd_t antHandleStart(void* antarg, cmd_t cmd) {
    ant_t ant = (ant_t) antarg;
	LOGPID("Ant handling start command.\n");
    ant->state = ANT_STATE_SEEKING;
    
    return NULL;
}

cmd_t antHandleTurn(void* antarg, cmd_t cmd) {
	ant_t ant = (ant_t) antarg;
	LOGPID("Ant handling turn command.\n");
	return (rand() % 2) ? newMoveReq(randDir()) : newSmellReq();

}

void antFillHandlerArray(handler_f* handlers) {
    
    handlers[CMD_START] = antHandleStart;
    handlers[CMD_TURN] = antHandleTurn;
    
    return;
}
