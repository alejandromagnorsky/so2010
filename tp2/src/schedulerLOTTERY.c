#include "../include/scheduler.h"

extern struct system_t System;

extern char scheduling;

task_t priorityRoundRobin();
int random();

task_t getNextTask()
{
    return scheduling ? priorityRoundRobin() : System.task;
}

task_t priorityRoundRobin()
{
	static int max = 0, high = 0, medium = 0, low = 0, min = 0, index = 0;
	
	int rand = random();
	
	_Cli();
	
	/*task_t candidate, new, old;
	
	static int left = 0, index = 0;
	
	static int times[NUM_TASKS];
    
	int counter = 0, auxi; 
	
	old = System.task;

	_Cli();
	
	if (old->tid != 0 && old->trank == RANK_SERVER) {
	    
	    if (old->tstatus == STATUS_READY)	
	    {
	    	printf("IS A SERVER");
		    return System.task;
	    }
		    
	}
	
	/*if(System.task->tid == 0)
	{
		return System.task;
	}*/
	
	/*if(left == 0) {
		candidate = NULL;
		
        if (old->tstatus == STATUS_RUNNING)
            old->tstatus = STATUS_READY;

		do
		{
			new = &(System.tasks[(++index) % NUM_TASKS]);
			if (new->tid != 0 && new->tstatus == STATUS_READY) {
			
			    if (new->trank == RANK_SERVER) {
			        candidate = new;
			        auxi = index;
    			    break;
    			    
			    } else if (candidate == NULL) {
    			    candidate = new;
    			    auxi = index;
			    }
			
			}
			
	    /* We wrap around the array as needed. When counter reaches NUM_TASKS,
		  we stop. */			
		/*} while(++counter < NUM_TASKS);
		
		index = auxi;		
		candidate = candidate ? candidate : System.idle;
			
		left = (CANT_PRIORITY - candidate->tpriority - 1) * RATIO;
		
		//printf("candidate tid: %d\n", candidate->tid);
		return candidate;
		
	} else {
		left--;
	    return System.task;
	}*/
	
	int counter = 0;

	do
	{
		index++; // index + min/max, etc

		if(System.tasks[index % NUM_TASKS]->tid > 1)
		{
			if (System.tasks[index % NUM_TASKS]->tstatus == STATUS_READY)
			{
				return &System.tasks[index % NUM_TASKS];
			}
		}
		counter++;
	}
	while (counter < NUM_TASKS);

	return System.idle;
	    
	
	if(rand < 50)
	{
		// choose a task with PRIORITY_MAX
	}
	else if (rand < 70)
	{
		// choose a task with PRIORITY_HIGH
	}
	else if (rand < 85)
	{
		// choose a task with PRIORITY_MEDIUM
	}
	else if(rand < 95)
	{
		// choose a task with PRIORITY_LOW
	}
	else
	{
		// choose a task with PRIORITY_MIN
	}
	
}

int random()
{
	static int prev = 89;
	return prev = (prev + 263) % 100;
}
