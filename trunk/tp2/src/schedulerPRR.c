#include "../include/scheduler.h"

extern struct system_t System;

extern char scheduling;

task_t priorityRoundRobin();

task_t getNextTask()
{
    return scheduling ? priorityRoundRobin() : System.task;
}

task_t dummyScheduler() {
    int i, stop;
    static int last = 0;
    
    i = stop = (last + 1) % NUM_TASKS;
    
    do {
    
        if (System.tasks[i].tid != 0)
            return System.tasks + (last = i);
        
        i = (i + 1) % NUM_TASKS;
        
    } while (i != stop);
    
    return System.idle;
    
}

task_t priorityRoundRobin()
{;
	task_t candidate, new, old;
	
	int counter = 0; 
    static int left = 0, index = 0;
	
	old = System.task;
	candidate = NULL;
	
	if (old->trank == RANK_SERVER) {
	    
	    if (old->tstatus == STATUS_READY)	
		    return System.task;
		    
	}

	if(left == 0) {

        if (old->tstatus == STATUS_RUNNING)
            old->tstatus = STATUS_READY;

		do
		{
			new = &(System.tasks[++index % NUM_TASKS]);
			
			if (new->tid != 0 && new->tstatus == STATUS_READY) {
			
			    if (new->trank == RANK_SERVER) {
			        candidate = new;
    			    break;
    			    
			    } else if (candidate == NULL)
    			    candidate = new;
			
			}
			
	    /* We wrap around the array as needed. When counter reaches NUM_TASKS,
		  we stop. */			
		} while(++counter < NUM_TASKS);
		
		candidate = candidate ? candidate : System.idle;
			
		left = (CANT_PRIORITY - candidate->tpriority - 1) * RATIO;
		
		return candidate;
		
	} else {
		left--;
	    return System.task;
	}
	
	
}

