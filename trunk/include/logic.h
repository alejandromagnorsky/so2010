/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       logic.h
**  Content:    structures and functions for server/ant logic.
*/

#include "cmd.h"

/* Pointer to a handler function: takes an inbound command and an ant ptr,
   and returns a reply command. [TODO] return a list of commands */
typedef cmd_t (*handler_f) (void*, cmd_t);

handler_f* buildAntHandlerArray();
