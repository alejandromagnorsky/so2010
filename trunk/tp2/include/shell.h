#ifndef	_SHELL_H_
#define	_SHELL_H_

#include "kasm.h"
#include "defs.h"
#include "klib.h"
#include "string.h"
#include "io.h"
#include "keyboard.h"
#include "kernel.h"
#include "cpuid.h"
#include "gralprotection.h"
#include "task.h"

void input_handler();
void parse_command();
int run_command();
void clearCommand();

int divideByZero(char*);
int help(char*);
int startx(char*);
int clear(char*);
int echo(char*);
int alloc(char*);

extern struct system_t System;
#endif
