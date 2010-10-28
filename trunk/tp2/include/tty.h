#ifndef _TTY_H_
#define _TTY_H_

#include "defs.h"
#include "screen.h"
#include "keyboard.h"


void _initialize_ttys(int);
void _refresh();
void _save();

extern tty_t ttys[NTTYS];
extern struct system_t System;
extern struct KBNamespace Keyboard;

struct TTYSNamespace {
	void (*initialize)();
	void (*refresh)();
	void (*save)();
	void (*load)();
	void (*jumpToTTY)(int);
	void (*runShells)();
	void (*switchTTY)(int);
};


#endif
