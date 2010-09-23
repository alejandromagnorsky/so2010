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

void input_handler();
void parse_command();
int run_command();
void clearCommand();

int divideByZero(char* line);
int help(char* line);
int startx(char * line);
int clear(char * line);
int echo(char * );
#endif
