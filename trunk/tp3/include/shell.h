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
#include "defs.h"
#include "at_wini.h"

void init();
void shell(int tty, linebuffer_t linebuffer, command_t command);

void input_handler();
void parse_command(linebuffer_t* linebuffer, command_t* command);
int run_command(command_t* command);
void clearCommand(command_t* command);
int get_command(command_t* command);
void getting_command(linebuffer_t* bufferinput);
int write_disk(char * a);

int divideByZero(char*);
int help(char*);
int startx(char*);
int clear(char*);
int echo(char*);
int alloc(char*);
int top(char*);
int demo_malloc(char * a);
int kill(char* tid);
int doNothing(char* line);
int echoserver(char*);
int ohceserver(char*);
int do_getchar(char* line);
int freeTest(char * line);
int check_drives(char * a);
int read_disk(char * a);
int write_disk(char * a);
int programDisk();


int daemontest(char * line);

extern struct system_t System;
extern tty_t ttys[NTTYS];
extern struct disk_t Disk; 

#endif
