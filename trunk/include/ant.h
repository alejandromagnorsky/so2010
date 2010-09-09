/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       ant.h
**  Content:    structures and functions related to the ants in the simulation.
*/

#ifndef __ANT_H__
#define __ANT_H__

#include "cmd.h"
#include "tools.h"
#include "logic.h"
#include <stdlib.h>

enum {
    ANT_STATE_ZERO,     /* Initial state. Simulation is not running. */
    ANT_STATE_IDLE,     /* Default behaviour. Will smell around. */
    ANT_STATE_SMELLED,
    ANT_STATE_FOLLOWING,/* Following a trail */
    ANT_STATE_FOUND,    /* Found food! */
    ANT_STATE_FOUNDBIG, /* Found BIG food! */
    ANT_STATE_YELLED,   /* Yelled for aid in carrying BIG food. It's BIG. */
    ANT_STATE_AIDING,   /* Heard a naerby yell, and decided to help. */
    ANT_STATE_CARRYING  /* Carrying food back to the anthill. */    
};

#define NUM_STATES 7

struct ant_t {
    int state;

    int x, y;       /* Ant's own position */
    int ahx, ahy;   /* Anthill position */
    int mx, my;     /* Memorized position */
};

typedef struct ant_t* ant_t;

ant_t antNew();
void antFree();

void antFillHandlerArray(handler_f*);

/* antHandleDispatch is just like the command handlers, only it takes an array
   of handlers as a 3rd argument. Returns what the appropiate handler rets,
   or NULL if no handler was found. */
cmd_t antDispatchCmd(ant_t, cmd_t, handler_f*);

#endif
