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
    _task_setPriority,
    _task_setRank,
    _task_setStatus,
    _task_getPriority,
    _task_getRank,
    _task_getStatus,
    _task_getTID,
    _task_new,
    _task_kill,
    _task_getById,
    _task_getCurrent,
    _task_getNewTID,
    _task_setupScheduler,
    _task_scheduler
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

int _task_setESP (task_t task, int esp) {
    task->esp = esp;
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

int _task_getESP (task_t task) {
    return task->esp;
}

int _task_findSlot() {
    int i;
    char found;
    
    /* Try and find a slot in the tasks array: */
	for (i = 0; i < NUM_TASKS; i++)
        if ( found = (System.tasks[i].tid == 0) ) break;

    return (found ? i : 0);

}

int _task_new (task_t task, char* name, program_t program, int rank, int priority) {
    /* Create a new task, given a program and its rank and priority */
    int i;
    char found;

	_Cli();

    /* Fill out basic task information */
    
	strcpy(name, task->tname);
	task->tid = Task.getNewTID();
	task->tpriority = priority;
	task->trank = rank;
    task->stack_size = DEFAULT_STACK_SIZE;

    /* Reserve memory for the stack (grows downwards in x86): */
    task->stack_start = task->stack =
        System.malloc(DEFAULT_STACK_SIZE) + DEFAULT_STACK_SIZE - 1;
    
    task->tstatus = STATUS_READY;
	
	_pageUp(task->stack);

    /* No idea why we need another esp variable. [TODO] ask Jime */
    task->esp = _newStack (program, task->stack_start, cleaner); 

    _pageDown(task->stack);
	_Sti();

    return task->tid;
}

/* Kills the given task */
// [TODO] check to awake parent and kill childs
void _task_kill(task_t task)
{
	_Cli();

    task->tstatus = STATUS_DEAD;
	/*_sys_free((*task)->stack);
	
	(*task)->free = 1;
	(*task)->tname[0] = '\0'; */
	
	_Sti();
}

/* Checks if the scheduler brings a new task, in that case it changes to the
	new one, otherwise it keeps running the current task */
void _task_scheduler()
{
	int stat;
	task_t old, new;

    new = old = _task_getCurrent(); /* Obtain currently running task */
    
	/* [TODO] Obtain next task according to scheduler */
	
	if(_task_getTID(new) != _task_getTID(old)) {
    
		_pageDown(old->stack);
        
		_task_save_state_();
		_task_load_state_(new->esp);
		
		_pageUp(new->stack);
		
		System.task = new;
		
	}
	/* saving process in last 100 executed [TODO] this should 
		be here or inside previous if?*/
		
	System.last100[System.last100Counter] = new->tid;
	increment100Counter();
	
	return;
}

task_t _task_getById(int tid) {
    /* Find a task by TID and return a pointer to its control block */
	int i;
    char found;
	
	for(i = 0; i < NUM_TASKS; i++)
        if (found = (System.tasks[i].tid == tid)) break;

    return found ? &(System.tasks[i]) : NULL;
}

task_t _task_getCurrent() {
    /* Return a pointer to the currently running task's control block */
	return System.task;
}

/* Idle task */
int idle (char* line) {
	for(;;);
}

/* Returns an unused tid */
int _task_getNewTID() {
	static int tid = 0;

    /* [TODO] fatal error possible here.
     * There could be overflow/wrapping problems in the long run...
    */
    return ++tid;
}

/* Tasks which finish end their life in this function which "frees" the space
	in the tasks array and calls the scheduler in order to run a new task */
static void cleaner(void)
{
	task_t task;
	
	_Cli();
	
	task = _task_getCurrent();
	
	task->tid = 0;
	task->tname[0] = '\0';
	
	_Sti();
	
	_scheduler();
}

/* Initializes the scheduler by creating the idle task and start running it */
void _task_setupScheduler ()
{
    task_t idle_task;
	/*
     * System.tasks array should already be zeroed
     * That is, all the IDs are set to 0, which is considered an empty
     * task slot. No extra initialization needed for that.
     */	

	/* What we do need to initialize is the idle task: */
    idle_task = &( System.tasks[_task_findSlot()] );
	_task_new(idle_task, "Idle", idle, RANK_NORMAL, PRIORITY_LOW);
    _task_setStatus(idle_task, STATUS_WAITING);
    
	return;
}

/* Saves the ESP for context changing */
void _saveESP(int esp)
{
	task_t task = _task_getCurrent();
	
	task->esp = esp;
	
	return;
}

/* Functions for top process */
int increment100Counter()
{
	System.last100Counter++;
	if(System.last100Counter >= LASTS_QUANT)
	{
		System.last100Counter = 0;
	}
}

int processCpuUsage(int tid)
{
	int i;
	int count = 0;

	for(i=0; i < LASTS_QUANT; i++)
	{
		if(System.last100[i] == tid)
		{
			count ++;
		}
	}

	return (count*100)/LASTS_QUANT;
}

void getStatusName(char* buffer, task_t task)
{
	if(_task_getStatus(task) == STATUS_RUNNING)
	{
		strcpy(buffer, "RUNNING");
	}	
	else if(_task_getStatus(task) == STATUS_READY)
	{
		strcpy(buffer, "READY");
	}
	else if(_task_getStatus(task) == STATUS_WAITING)
	{
		strcpy(buffer, "WAITING");
	}
	else if(_task_getStatus(task) == STATUS_DEAD)
	{
		strcpy(buffer, "DEAD");
	}
	return;
}

void getRankName(char* buffer, task_t task)
{
	if(_task_getRank(task) == RANK_SERVER)
	{
		strcpy(buffer, "SERVER");
	}	
	else if(_task_getRank(task) == RANK_NORMAL)
	{
		strcpy(buffer, "NORMAL");
	}
	return;
}


int top()
{
	printf("Processes and it's CPU percentage of use\n");
	
	int i;
	char status[10];
	char rank[10];
	
	for(i=0; i < NUM_TASKS; i++)
	{
		if(System.tasks[i].tid == 0)
		{
			getStatusName(status, &(System.tasks[i]));
			getRankName(rank, &(System.tasks[i]));

			printf("%s [%s], pid: %d, priority: %d, rank: %s, use: %d%%\n",
				System.tasks[i].tname, status, System.tasks[i].tid, 
				System.tasks[i].tpriority, rank, 
				processCpuUsage(System.tasks[i].tid));
		}
	}
	printf ("\n");

	return 0;
	
}
