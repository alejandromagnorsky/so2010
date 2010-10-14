#include "../include/scheduler.h"

extern struct system_t System;

static int timesLeft = 0;
static int index = 0;

task_t priorityRoundRobin();

task_t getNextTask()
{
	return priorityRoundRobin();
}

task_t priorityRoundRobin()
{
	int counter = 0;
	task_t task;
	
	if(timesLeft == 0)
	{
		do
		{
			task = &(System.tasks[index % NUM_TASKS]);
			index ++;
			if(task->tid != 0)
			{
				if(task->trank != RANK_SERVER)
				{
					if(task->tstatus == STATUS_READY)
					{
						timesLeft = (CANT_PRIORITY - task->tpriority -1) * RATIO;
						return task;
					}
				}
			}
			counter ++;
		}
		while(counter < NUM_TASKS);
		return System.idle;
	}
	else
	{
		timesLeft --;
	}
	return task;
	
}

