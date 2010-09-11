/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       ant.h
**  Content:    structures and functions related to the ants in the simulation.
*/

#ifndef __ANT_H__
#define __ANT_H__

#include "ipc.h"
#include "cmd.h"
#include "tools.h"
#include <stdlib.h>
#include <math.h>

#define COINFLIP() (rand() % 2)
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

enum {
    ANT_STATE_ZERO,     /* Initial state. Simulation is not running. */
    ANT_STATE_SEEKING,  /* Default behaviour. Will smell around. */
    ANT_STATE_WAITING,  /* Yelled for aid in carrying BIG food. It's BIG. */
    ANT_STATE_GOING,    /* Heard a naerby yell, and decided to help. */
    ANT_STATE_AIDING,   /* Got there, looking for the big food. */
    ANT_STATE_CARRYING, /* Carrying food back to the anthill. */ 
    ANT_STATE_FINAL     /* Simulation has ended. */
};

#define NUM_STATES 6

struct ant_t {
    int state;

    int pickdir;    /* Waiting next to big food in this direction */
    int r, c;       /* Ant's own position */
    int ahr, ahc;   /* Anthill position */
    int mr, mc;     /* Memorized position */
    
    char smelled;
    struct tile_t smell[8];
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
