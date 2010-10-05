#ifndef _TTY_H_
#define _TTY_H_

#include "keyboard.h"
#include "screen.h"

#define NTTYS 4

typedef struct {
	flags_t flags;
}input_t;

typedef screen_t output_t; //Si necesitamos mas hacemos una struct

typedef struct {
	input_t input;
	output_t output;
}tty_t;


void initialize_ttys();
void initialize_tty(tty_t * tty);

#endif
