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
#include <time.h>
#include <math.h>

#define COINFLIP() (rand() % 2)
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

enum {
    ANT_STATE_ZERO,     /* Initial state. Simulation is not running. */
    ANT_STATE_SEEKING,  /* Default behaviour. Will smell around. */
    ANT_STATE_WAITING,  /* Waiting for help with big food */
    ANT_STATE_CARRYING, /* Carrying food back to the anthill. */ 
    ANT_STATE_FINAL     /* Simulation has ended. */
};

#define NUM_STATES 5

struct ant_t {
    int state;

    int r, c;       /* Ant's own position */
    int ahr, ahc;   /* Anthill position */
    int mr, mc;     /* Memorized position */
    
    char yelled;    /* Did we already yell when we found this food? */
    char smelled;   /* Flag to remember whether smell data is still valid */
    struct tile_t smell[NUM_DIRS];

    double interestbase[NUM_DIRS];
    double interestmult[NUM_DIRS];
};

typedef struct ant_t* ant_t;

ant_t antNew();
void antFree();
int decide(double* base, double* mult);

void antFillHandlerArray(handler_f*);

/* antHandleDispatch is just like the command handlers, only it takes an array
   of handlers as a 3rd argument. Returns what the appropiate handler rets,
   or NULL if no handler was found. */
cmd_t antDispatchCmd(ant_t, cmd_t, handler_f*);

#endif
