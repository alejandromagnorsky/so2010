/*
** Task header file
*/

#ifndef __TASK_H__
#define __TASK_H__

#include "klib.h"

extern struct system_t System;
extern struct TaskNamespace Task;

int testTasks(char * a);
int task1 (char* line);
int task2 (char* line);
int task3 (char* line);

int doNothing4Times(char* line);
int doNothing2(char * line);
int doNothing3(char * line);
int doNothing4(char * line);

#endif
