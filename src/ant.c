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
    message_t message, ret;

    ant = antNew();
    LOGPID("Starting ant logic loop.\n");

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
            if (ret = dispatchCmd((void*) ant, (cmd_t) mdata(message), handlers))
                sendMessage(ipc, ret);
            
        }
    
    }
}

cmd_t antHandleStart(void* antarg, cmd_t cmdarg) {
    ant_t ant = (ant_t) antarg;
	LOGPID("Ant handling start command.\n");
    ant->state = ANT_STATE_SEEKING;
    
    return NULL;
}

cmd_t antHandleTurn(void* antarg, cmd_t cmdarg) {
    int dir;
	ant_t ant = (ant_t) antarg;
	LOGPID("Ant handling turn command.\n");
	
	switch (ant->state) {
	    case ANT_STATE_SEEKING:
	        if (ant->smelled) {
	            for (dir = DIR_NORTH; dir <= DIR_NORTHWEST; dir++) {
	                if (ant->smell[dir].obj == OBJ_FOOD)
	                    return newPickReq(dir);
	                    
	        } else
    	        return (rand() % 2) ? newMoveReq(randDir()) : newSmellReq();        
	
        case 

}

cmd_t antHandleSmellRes(void* antarg, cmd_t cmdarg) {
    ant_t ant = (ant_t) antarg;
    cmd_smell_res_t cmd = (cmd_smell_res_t) cmdarg;
    
    ant->smelled = 1;
    memcpy(ant->smell, cmd->tiles, sizeof(struct tile_t) * NUM_DIRS);
    
    return NULL;
}

cmd_t antHandlePickRes(void* antarg, cmd_t cmdarg) {
    ant_t ant = (ant_t) antarg;
    cmd_pick_res_t cmd = (cmd_pick_res_t) cmdarg;
    
    if (status == STATUS_OK) {
        
    }
}

void antFillHandlerArray(handler_f* handlers) {
    
    handlers[CMD_START] = antHandleStart;
    handlers[CMD_TURN] = antHandleTurn;
    handlers[CMD_SMELL_RES] = antHandleSmellRes;
    
    return;
}
