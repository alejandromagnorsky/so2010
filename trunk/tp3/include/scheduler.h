#ifndef _scheduler_
#define _scheduler_

#include "./klib.h"

/* This function returns the next process to be executed, depending on the 
	scheduling algorithm used. */

task_t getNextTask();

#endif
