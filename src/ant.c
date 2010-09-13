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
    LOGPID("Starting ant %d logic loop.\n", ipc->id);

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
            LOGPID("Ant %d received cmd type %d.\n", ipc->id, ((cmd_t) mdata(message))->type);
            if (cmd = dispatchCmd((void*) ant, (cmd_t) mdata(message), handlers)) {
                mdel(message);

                sendMessage(ipc, message = mnew(ipc->id, 1, cmdsize(cmd),
                                               (char*) cmd));

                LOGPID("Ant %d sent: ", ipc->id);
                mprintln(message);
                mdel(message);                         
            }
        }
        
        stop = (ant->state == ANT_STATE_FINAL);
    
    }
    
    ipc->stop = 1;
    return 0;
}

cmd_t antHandleStart(void* antarg, cmd_t cmdarg) {
    ant_t ant = (ant_t) antarg;
	LOGPID("Ant handling start command.\n");
    ant->state = ANT_STATE_SEEKING;
    
    return NULL;
}

cmd_t antHandleTurn(void* antarg, cmd_t cmdarg) {
    int dir, dist;
	ant_t ant = (ant_t) antarg;
	LOGPID("Ant handling turn command.\n");
	
	switch (ant->state) {
	
	   case ANT_STATE_SEEKING:
	   
	        if (ant->smelled) {
	            /* Smell memory is still valid */
	            
	            for (dir = DIR_NORTH; dir < DIR_NORTHWEST; dir++) {
	            
	                switch(ant->smell[dir].obj) {
	                
	                    case OBJ_FOOD:
    	                    return newPickReq(dir);
    	                    
	                    case OBJ_BIGFOOD:
                            return (ant->yelled) ? newPickReq(dir) : newYellReq();

	                    case OBJ_ANT:    
	                        ant->interestbase[dir] = 0;
	                        break;
	                        
	                    default:
	                        ant->interestbase[dir] = 50;
	                        break;
	                }
	                
	                ant->interestmult[dir] /= ant->smell[dir].trail;
	            }
	            
	        } else
                if (rand() % 3) return newSmellReq();
                
            return newMoveReq(decide(ant->interestbase, ant->interestmult));    
	
        case ANT_STATE_CARRYING:
            if ((dist = ant->r - ant->ahr) != 0) {
            
                dir = (dist > 0 ? DIR_NORTH : DIR_SOUTH);
                ant->interestbase[dir] = 150;
                
            } else if ((dist = ant->c - ant->ahc) != 0) {
            
                dir = (dist > 0 ? DIR_WEST : DIR_EAST);
                ant->interestbase[dir] = 150;
                
            } else {
                ant->state = ANT_STATE_SEEKING;
                return NULL;
            }
            
            return newMoveReq(decide(ant->interestbase, ant->interestmult));
    }
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
    
    switch(cmd->status) {
        case STATUS_FAILED:
            ant->smelled = 0;
            break;

        default:
        case STATUS_OK:
            ant->yelled = 0;
            ant->state = ANT_STATE_CARRYING;
            break;
            

    }
    
    return NULL;
}

cmd_t antHandleMoveRes(void* antarg, cmd_t cmdarg) {
    ant_t ant = (ant_t) antarg;
    cmd_pick_res_t cmd = (cmd_pick_res_t) cmdarg;
    
    switch(cmd->status) {
    
        case STATUS_OK:
            ant->smelled = 0;
            break;
            
        case STATUS_FAILED:
            break;
    
    }
    
    return NULL;
}

cmd_t antHandleYellRes(void* antarg, cmd_t cmdarg) {
    ant_t ant = (ant_t) antarg;
    ant->yelled = 1;
    
    return NULL;
}

cmd_t antHandleYellNot(void* antarg, cmd_t cmdarg) {
    int dirr, dirc, distr, distc;
    ant_t ant = (ant_t) antarg;
    cmd_yell_not_t cmd = (cmd_yell_not_t) cmdarg;
    
    if ((distr = ant->r - cmd->r) != 0)
        ant->interestmult[dirr = (distr > 0) ? DIR_NORTH : DIR_SOUTH] *= 2;

    if ((distc = ant->c - cmd->c) != 0)
        ant->interestmult[dirc = (distc > 0) ? DIR_WEST : DIR_EAST] *= 2;
    
    return NULL;
    
}

cmd_t antHandleStop(void* antarg, cmd_t cmdarg) {
    ant_t ant = (ant_t) antarg;
    ant->state = ANT_STATE_FINAL;
    return newStop();
}

int decide(double* base, double* mult) {
    int i;
    double random;
    double accum, total;
    double interest[NUM_DIRS];
    
    for (i = 0; i < NUM_DIRS; i++)
        interest[i] = base[i] * mult[i];
    
    /* We'll try to simulate a real given random distribution */    
    
    for(total = i = 0; i < NUM_DIRS; i++)
        total += interest[i];
        
    random = rand() / (double) (RAND_MAX) * total;
        
    for(accum = i = 0; i < NUM_DIRS; i++) {
        accum += interest[i];
        
        if (random <= accum)
            return (i + 1) % NUM_DIRS;        
        
    }
    
    return 150; /* ? */
}

void antFillHandlerArray(handler_f* handlers) {
    
    handlers[CMD_START] = antHandleStart;
    handlers[CMD_TURN] = antHandleTurn;
    handlers[CMD_SMELL_RES] = antHandleSmellRes;
    handlers[CMD_MOVE_RES] = antHandleMoveRes;
    handlers[CMD_PICK_RES] = antHandlePickRes;
    handlers[CMD_YELL_RES] = antHandleYellRes;
    handlers[CMD_YELL_NOT] = antHandleYellNot;
    handlers[CMD_STOP] = antHandleStop;
    return;
}
