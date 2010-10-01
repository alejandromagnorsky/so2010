#include "../include/klib.h"

extern struct system_t System;



/************************************************
** System data structure manipulation:
*************************************************/

size_t _sys_write(int devcode, void* from, size_t nbytes) {
    size_t ret;
    
    MOVTO_AL(SYSTEM_CALL_WRITE);
    MOVTO_EBX(devcode)
    MOVTO_ECX(from);
    MOVTO_EDX(nbytes);
    
    THROW_INT80;
    
    MOVFROM_EAX(ret);
    return ret;
}

size_t _sys_read(int devcode, void* to, size_t nbytes) {
    size_t ret;
    
    MOVTO_AL(SYSTEM_CALL_READ);
    MOVTO_EBX(devcode)
    MOVTO_ECX(to);
    MOVTO_EDX(nbytes);
    
    THROW_INT80;
    
    MOVFROM_EAX(ret);
    return ret;
}

size_t _sys_seekr(int devcode, int offset, int from) {
    size_t ret;

    MOVTO_AL(SYSTEM_CALL_SEEKR);
    MOVTO_EBX(devcode);
    MOVTO_ECX(offset);
    MOVTO_EDX(from);
    
    THROW_INT80;
    
    MOVFROM_EAX(ret);
    return ret;
}

size_t _sys_seekw(int devcode, int offset, int from) {
    size_t ret;
    
    MOVTO_AL(SYSTEM_CALL_SEEKW);
    MOVTO_EBX(devcode);
    MOVTO_ECX(offset);
    MOVTO_EDX(from);
    
    THROW_INT80;
    
    MOVFROM_EAX(ret);
    return ret;
}

size_t _sys_tellr(int devcode) {
    size_t ret;
    
    MOVTO_AL(SYSTEM_CALL_TELLR);
    MOVTO_EBX(devcode);
    
    THROW_INT80;
    
    MOVFROM_EAX(ret);
    return ret;
}

size_t _sys_tellw(int devcode) {
    size_t ret;
    
    MOVTO_AL(SYSTEM_CALL_TELLW);
    MOVTO_EBX(devcode);
    
    THROW_INT80;
    
    MOVFROM_EAX(ret);
    return ret;
}

int _sys_exec(int (*f) (char*), char* args) {
    int ret;
    
    MOVTO_AL(SYSTEM_CALL_EXEC);
    MOVTO_EBX(f);
    MOVTO_ECX(args);
    
    THROW_INT80;
    
    MOVFROM_EAX(ret)
    return ret;
}




struct TaskNamespace Task = {
    (void*) 0,
    _task_setPriority,
    _task_setRank,
    _task_setStatus,
    _task_getPriority,
    _task_getRank,
    _task_getStatus,
    _task_getTID,
    //_task_createNewTask,
    _task_getNextTask,
    _task_getTaskById,
    _task_getCurrentTask,
    _task_getNewTID,
    _task_setupScheduler
};


/* Basic getters/setters: */

void _task_setPriority (task_t task, int tpriority) {
    if (task != NULL && tpriority >= PRIORITY_LOW && tpriority <= PRIORITY_MAX)
        task->tpriority = tpriority;
        
    return;
}

void _task_setRank (task_t task, int trank) {
    if (task != NULL && trank >= RANK_SERVER && trank <= RANK_NORMAL)
        task->trank = trank;

    return;
}

void _task_setStatus (task_t task, int tstatus) {
    if (task != NULL && tstatus >= STATUS_READY && tstatus <= STATUS_RUNNING)
        task->tstatus = tstatus;

    return;
}

int _task_getPriority (task_t task) {
    return task->tpriority;
}

int _task_getRank (task_t task) {
    return task->trank;
}

int _task_getStatus (task_t task)
{
    return task->tstatus;
}

int _task_getTID (task_t task) {
    return task->tid;
}

void _task_createNewTask (char* name, int (*task) (void), int priority, int status)
{
	int i;
	void* stack;
	
	_Cli();
	
	/* Looking for a free place */
	for (i=0; i<NUM_TASKS; i++)
	{
		if (System.tasks[i].free == 1)
		{
			break;
		}
	}
	
	strcpy(name, System.tasks[i].tname);
	System.tasks[i].tid = Task.getNewTID();
	System.tasks[i].tpriority = priority;
	System.tasks[i].tstatus = status;
	
	/* Alloc memory for stack */
	// [TODO] check error in following line
	//stack = _reqpage(System.tasks[i]);
	System.tasks[i].stack = stack;
	// [TODO] PAGE_SIZE should be PAGESIZE
	System.tasks[i].stackSize = PAGE_SIZE;
	System.tasks[i].stackStart = ((int)stack) + System.tasks[i].stackSize - 1;
	
	// [TODO]  pages up
	System.tasks[i].esp = _newStack (task, System.tasks[i].stackStart, cleaner ); 
	// [TODO]  pages down
	
	_Sti();
}

void _task_getNextTask()
{
	
	task_t* oldTask = Task.getCurrentTask(); /* Obtain currently running task */
	task_t* newTask; /* Obtain next task according to scheduler */
	
	if(Task.getTID(*oldTask) != Task.getTID(*newTask))
	{
		/* [TODO] bajar las pages de la vieja task */
		_task_save_state_();
		_task_load_state_((*newTask)->esp);
		
		/* [TODO] subir las pages de la nueva task */
	}
	
	return;
}

struct task_t _task_getTaskById(int tid)
{
	int i;
	
	/*if(tid == 0)
	{
		return System.idle;
	}*/
	
	for( i = 0; i < NUM_TASKS; i++)
	{
		if(System.tasks[i].tid == tid)
		{
			return System.tasks[i];
		}
	}

}

task_t* _task_getCurrentTask()
{
	return System.task;
}


static int Idle (void)
{
	while(1)
	{
		;
	}
}

int _task_getNewTID()
{
	return System.nextTID++;
}

static void cleaner(void)
{
	task_t* task;
	
	_Cli();
	
	task = Task.getCurrentTask();
	
	(*task)->tname[0] = '\0';
	(*task)->free = 1;
	
	_Sti();
	
	_scheduler();
}


void _task_setupScheduler ()
{
	void* idleStack;
	int i;

	/* Tasks array initialized with free places */

	for (i=0; i<NUM_TASKS; i++)
	{
		System.tasks[i].tid = -1;
		System.tasks[i].free = 1;
	}

	/* Idle task is initialized */

	strcpy("Idle", System.idle->tname);
	System.idle->tid = 0;
	System.idle->free = 0;
	
	// [TODO] which is the idle's task priority?
	//System.idle->tpriority = ???;
	
	// [TODO] which is the idle's task rank?
	//System.idle->trank = ???;
	
	// [TODO] idle's task status should be running?
	//System.idle->tstatus = ???;

	/* Stack memory reserved and task stack created */

	//[TODO] check error of this function
	//idleStack = _reqpage(Idle);
	//System.idle->stack = idleStack;
	System.idle->stackSize = PAGE_SIZE;
	System.idle->stackStart =  ((int)idleStack) + System.idle->stackSize - 1;

	// [TODO]  pages up
	System.tasks[i].esp = _newStack (Idle, System.idle->stackStart, cleaner ); 
	// [TODO]  pages down
	
	return;
}


