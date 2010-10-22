#include "../include/scheduler.h"

extern struct system_t System;

extern char scheduling;

task_t priorityRoundRobin();

task_t getNextTask()
{
    return scheduling ? priorityRoundRobin() : System.task;
}

/* Dummy scheduler is working */
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
	
	static int left = 0, index = 0;
    
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
	
	if(left == 0) {
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
		} while(++counter < NUM_TASKS);
		
		index = auxi;		
		candidate = candidate ? candidate : System.idle;
			
		left = (CANT_PRIORITY - candidate->tpriority - 1) * RATIO;
		
		//printf("candidate tid: %d\n", candidate->tid);
		return candidate;
		
	} else {
		left--;
	    return System.task;
	}
}

