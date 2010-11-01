#include "../include/scheduler.h"

extern struct system_t System;

extern char scheduling;

task_t lottery();
task_t findTask(int priority);
int random();

task_t getNextTask()
{
    return scheduling ? lottery() : System.task;
}

task_t lottery()
{
	int rand = random();
	
	task_t task, old = System.task;
	
	if (old->tid != 0 && old->trank == RANK_SERVER) {
	    
	    if (old->tstatus == STATUS_RUNNING)	
	    {
		    return System.task;
	    }
		    
	}
	
	if (old->tstatus == STATUS_RUNNING)
            old->tstatus = STATUS_READY;
            
    /* It gives higher priority tasks more chances of running, and chances
       decrement as priorities do */
	if(rand < 50)
	{
		/* If there is one ready, choose a task with PRIORITY_MAX, if not look
		   for another one in priority order */
		
		task = findTask(PRIORITY_MAX);
		if(task != System.idle)
		{
			return task;
		}
		else
		{
			task = findTask(PRIORITY_HIGH);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MEDIUM);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_LOW);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MIN);
			if(task != System.idle)
			{
				return task;
			}
		}
	}
	else if (rand < 70)
	{
		/* If there is one ready, choose a task with PRIORITY_HIGH, if not look
		   for another one in priority order */
		
		task = findTask(PRIORITY_HIGH);
		if(task != System.idle)
		{
			return task;
		}
		else
		{
			task = findTask(PRIORITY_MAX);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MEDIUM);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_LOW);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MIN);
			if(task != System.idle)
			{
				return task;
			}
		}
	}
	else if (rand < 85)
	{
		/* If there is one ready, choose a task with PRIORITY_MEDIUM, if not look
		   for another one in priority order */
		
		task = findTask(PRIORITY_MEDIUM);
		if(task != System.idle)
		{
			return task;
		}
		else
		{
			task = findTask(PRIORITY_MAX);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_HIGH);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_LOW);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MIN);
			if(task != System.idle)
			{
				return task;
			}
		}
	}
	else if(rand < 95)
	{
		/* If there is one ready, choose a task with PRIORITY_LOW, if not look
		   for another one in priority order */
		
		task = findTask(PRIORITY_LOW);
		if(task != System.idle)
		{
			return task;
		}
		else
		{
			task = findTask(PRIORITY_MAX);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_HIGH);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MEDIUM);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MIN);
			if(task != System.idle)
			{
				return task;
			}
		}
	}
	else
	{
		/* If there is one ready, choose a task with PRIORITY_MIN, if not look
		   for another one in priority order */
		
		task = findTask(PRIORITY_MIN);
		if(task != System.idle)
		{
			return task;
		}
		else
		{
			task = findTask(PRIORITY_MAX);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_HIGH);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_MEDIUM);
			if(task != System.idle)
			{
				return task;
			}
			task = findTask(PRIORITY_LOW);
			if(task != System.idle)
			{
				return task;
			}
		}
	}
	
	return System.idle;
}

/* Looks for a task ready to run with the given priority */
task_t findTask(int priority)
{
	static int max = 0, high = 0, medium = 0, low = 0, min = 0;
	task_t task;
	int counter = 0;
	do
	{
		if(priority == PRIORITY_MAX)
		{
			max ++;
			task = &System.tasks[max % NUM_TASKS];
		}
		else if (priority == PRIORITY_HIGH)
		{
			high ++;
			task = &System.tasks[high % NUM_TASKS];
		}
		else if (priority == PRIORITY_MEDIUM)
		{
			medium ++;
			task = &System.tasks[medium % NUM_TASKS];
		}
		else if(priority == PRIORITY_LOW)
		{
			low ++;
			task = &System.tasks[low % NUM_TASKS];
		}
		else
		{
			min ++;
			task = &System.tasks[min % NUM_TASKS];
		}
		
		
		if(task->tid > 1)
		{
			if (task->tpriority == priority && 
				(task->tstatus == STATUS_READY))
			{
					return task;
			}
		}
		counter++;
	}
	while (counter < NUM_TASKS);

	return System.idle;
}

int random()
{
	static int prev = 89;
	return prev = (prev + 263 + System.ticks * 5) % 100;
}
