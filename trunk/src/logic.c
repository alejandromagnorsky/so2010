/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       logic.c
**  Content:    structures and functions for server/ant logic.
*/

#include "../include/logic.h"

handler_f* buildHandlerArray() {
    return (handler_f*) calloc(NUM_CMDS, sizeof(handler_f));
}

cmd_t dispatchCmd(void* obj, cmd_t cmd, handler_f* handlers) {

    if (cmd->type >= 0 && cmd->type < NUM_CMDS)
        if (handlers[cmd->type] != NULL)
            return handlers[cmd->type](obj, cmd);
    
    /* Invalid command type, or no handler specified for it: */
    return NULL;

}

int randDir() {
    return rand() % NUM_DIRS;
}
