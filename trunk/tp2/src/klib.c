#include "../include/klib.h"

#pragma pack(1)

extern struct system_t System;
extern char scheduling;


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
    _task_findSlot,
    _task_new,
    _task_kill,
    _task_getByTID,
    _task_getCurrent,
    _task_getNewTID,
    _task_setupScheduler,
    _task_scheduler,
    _task_cleaner,
	_task_setTty,
	_task_getTty,
	_task_runInBackground,
	_task_getRunningMode,
	_task_yield
};

struct TopNamespace Top = {
	_top_increment100Counter,
	_top_processCpuUsage,
	_top_getStatusName,
	_top_getRankName,
	_top_getPriority,
	_top_initialize,
	_top_clearTask,
	_top_run
};


/* Basic getters/setters: */

void _task_setPriority (task_t task, int tpriority) {
    if (task != NULL && tpriority <= PRIORITY_NEVER && tpriority >= PRIORITY_MAX)
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

void _task_setTty(task_t task, int tty){
	task->tty = tty;
}

int _task_getTty(task_t task){
	return task->tty;
}

void _task_runInBackground(task_t task){
	task->running_mode = RUNNING_BACK;
}

int _task_getRunningMode(task_t task){
	return task->running_mode;
}

int _task_findSlot() {
    int i;
    char found;
    
    /* Try and find a slot in the tasks array: */
	for (i = 0; i < NUM_TASKS; i++)
        if ( found = (System.tasks[i].tid == 0) ) break;

    return (found ? i : 0);

}

int _task_new (task_t task, char* name, program_t program, int rank, 
			int priority, int isFront) {
    /* Create a new task, given a program and its rank and priority */
    int i;
    char found;
    
	_Cli();

    task_t current = Task.getCurrent();

    /* Fill out basic task information */
    
	strcpy(name, task->tname);
	task->tid = Task.getNewTID();
	Task.setPriority(task, priority);
	Task.setRank(task, rank);
    task->stack_size = DEFAULT_STACK_SIZE;

    /* Reserve memory for the stack (grows downwards in x86): */
    task->stack = System.malloc(DEFAULT_STACK_SIZE);
    task->stack_start = task->stack + DEFAULT_STACK_SIZE - 1;
    
    if(rank == RANK_SERVER)
    {
		Task.setStatus(task, STATUS_WAITING);
    }
    else 
    {
	    Task.setStatus(task, STATUS_READY);
    }
	
	//_pageUp(task->stack);

    task->esp = _newStack (program, task->stack_start, Task.cleaner);
	//_pageDown(task->stack);
	
	// [TODO] check this
	if(isFront && current->tid > 1)
	{
		current->tstatus = STATUS_WAITING;
		task->parentTID = current->tid;
	}
	else
	{
		task->parentTID = 0;
		//Task.runInBackground(task);
	}
	
	_Sti();
	
    return task->tid;
}

/* Kills the given task */
// [TODO] check this
void _task_kill(task_t task)
{
	int i;
	task_t parent;

	_Cli();
	
	if(task->tid == 0)
	{
		return;
	}
	
	/* Looking for task's children in order to kill them */
	for(i = 0; i < NUM_TASKS; i++)
	{
		if(System.tasks[i].tid != 0 && System.tasks[i].parentTID == task->tid)
		{
			Task.kill(&System.tasks[i]);
		}
	}
	
	/* Need to awake parent if it's not idle and it's waiting */
	if(task->parentTID != 0)
	{
		parent = Task.getByTID(task->parentTID);
		Task.setStatus(parent, STATUS_READY);
	}

    Task.setStatus(task, STATUS_DEAD);
    task->tid = 0;
    task->tname[0] = '\0';
	/*_sys_free((*task)->stack);*/
	
	
	_Sti();
}

/* Checks if the scheduler brings a new task, in that case it changes to the
	new one, otherwise it keeps running the current task */
int _task_scheduler(int esp)
{
    int i;
    static int stop = 0;
	task_t old, new;
	
    old = Task.getCurrent();       /* Obtain currently running task */
    new = (task_t) getNextTask(); 
    
    old->esp = esp;
    
    /*_Cli();
    if (++stop < 10) {
        printf("Old task id: %d (%s) status: %d\n", old->tid, old->tname, old->tstatus);
        printf("New task id: %d (%s) status: %d\n", new->tid, new->tname, new->tstatus);
    }
    _Sti();*/

	if(Task.getTID(new) != Task.getTID(old)) {
		System.task = new;

		/*_pageDown(old->stack);
		_pageUp(new->stack);
		*/

	    if (old->tstatus == STATUS_RUNNING)
	        Task.setStatus(old, STATUS_READY);

	    if (new != System.idle)        
    	    Task.setStatus(new, STATUS_RUNNING);
	    
	}
	
	/* saving process in last 100 executed*/
	System.last100[System.last100Counter] = new->tid;
	Top.increment100Counter();
	
	//printf("%d\n", new->tid);

	return new->esp;
}

task_t _task_getByTID(int tid) {
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
	   //printf("%d ", System.idle->tid);
}

// [TODO] remove this
int task1 (char* line) {
    int i = 0;
    printf("task 1 started... \n");
    while(System.ticks < 10)
    {
    	//Top.run();
    }
	printf("task 1 finishing... \n");
    return 1;
}

int task2 (char* line) {
    int i = 0;
    printf("task 2 started... \n");
    printf("task 2 killing task 3...\n");
    Task.kill(&(System.tasks[1]));
    printf("task 3 killed...\n");    
    while(System.ticks < 20)
    {
    	//Top.run();
    }
	printf("task 2 finishing... \n");
    return 1;
}

int task3 (char* line) {
    int i = 0;
    printf("task 3 started... \n");
    while(System.ticks < 30)
    {
    	//Top.run();
    }
	printf("task 3 finishing... \n");
    return 1;
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
	in the tasks array, awakes the task's parent if necessary and calls the 
	scheduler in order to run a new task */
static void _task_cleaner(void)
{
	
	_Cli();
	task_t task, parent;
	task = Task.getCurrent();
	
	/* Awake parent if necessary */
	if(task->parentTID > 1)
	{
		parent = Task.getByTID(task->parentTID);
		parent->tstatus = STATUS_READY;
	}
	
	task->tname[0] = '\0';
	Task.setStatus(task, STATUS_DEAD);
	task->tid = 0;
	
	Top.clearTask(task->tid);
	
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
     * task slot.
     */	

	/* What we need to initialize is the idle task: */
	
	
    idle_task = &(System.tasks[Task.findSlot()] );
	Task.new(idle_task, "Idle", idle, RANK_NORMAL, PRIORITY_NEVER, 0);
    Task.setStatus(idle_task, STATUS_WAITING);
    
    Task.new(&(System.tasks[Task.findSlot()]), "Task 3", task3, RANK_NORMAL, PRIORITY_HIGH, 0);
    Task.new(&(System.tasks[Task.findSlot()]), "Task 1", task1, RANK_NORMAL, PRIORITY_LOW, 0);
    Task.new(&(System.tasks[Task.findSlot()]), "Task 2", task2, RANK_NORMAL, PRIORITY_LOW, 0);
    
    System.task = System.idle = idle_task;
    
	Top.initialize(idle_task->tid);
	
    scheduling = 1;
	return;
}

void _task_yield(task_t task)
{
	// [TODO] should this cli be here, and is it right not to incude a sti?
	_Cli();
	Task.setStatus(task, STATUS_WAITING);
	_Sti();
	_scheduler();
}

/* Functions for top process */
int _top_increment100Counter()
{
	System.last100Counter++;
	if(System.last100Counter >= LASTS_QUANT)
	{
		System.last100Counter = 0;
	}
}

int _top_processCpuUsage(int tid)
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

void _top_getStatusName(char* buffer, task_t task)
{
    /* [TODO] usar un array aca. Dale, Jime. Hace falta llamar a la funcion
    *  cada vez que queres el status? Existen las variables. Jatejode. */
	if(Task.getStatus(task) == STATUS_RUNNING)
	{
		strcpy("RUNNING", buffer);
	}	
	else if(Task.getStatus(task) == STATUS_READY)
	{
		strcpy("READY", buffer);
	}
	else if(Task.getStatus(task) == STATUS_WAITING)
	{
		strcpy("WAITING", buffer);
	}
	else if(Task.getStatus(task) == STATUS_DEAD)
	{
		strcpy("DEAD", buffer);
	}
	return;
}

void _top_getRankName(char* buffer, task_t task)
{
	if(Task.getRank(task) == RANK_SERVER)
	{
		strcpy("SERVER", buffer);
	}	
	else if(Task.getRank(task) == RANK_NORMAL)
	{
		strcpy("NORMAL", buffer);
	}
	return;
}

void _top_getPriority(char* buffer, task_t task)
{
	if(Task.getPriority(task) == PRIORITY_LOW)
	{
		strcpy("LOW", buffer);
	}	
	else if(Task.getPriority(task) == PRIORITY_HIGH)
	{
		strcpy("HIGH", buffer);
	}
	else if(Task.getPriority(task) == PRIORITY_NEVER)
	{
		strcpy("NEVER", buffer);
	}
	else if(Task.getPriority(task) == PRIORITY_MEDIUM)
	{
		strcpy("MEDIUM", buffer);
	}
	else if(Task.getPriority(task) == PRIORITY_MAX)
	{
		strcpy("MAX", buffer);
	}
	return;
}

void _top_initialize(int tid)
{
	int i;
	
	for(i=0; i < LASTS_QUANT; i++)
	{
		System.last100[i] = tid;
	}
}

void _top_clearTask(int tid)
{
	int i;
	
	for(i=0; i < LASTS_QUANT; i++)
	{
		if(System.last100[i] == tid)
		{
			System.last100[i] = System.idle->tid;
		}
	}
}

int _top_run()
{
	printf("Processes and it's CPU percentage of use\n");
	
	int i;
	char status[10];
	char rank[10];
	char priority[10];
	
	_Cli();
	
	for(i=0; i < NUM_TASKS; i++)
	{
		if(System.tasks[i].tid != 0)
		{
			Top.getStatusName(status, &(System.tasks[i]));
			Top.getRankName(rank, &(System.tasks[i]));
			Top.getPriority(priority, &(System.tasks[i]));

			printf("%s [%s] \t tid: %d\t priority: %s\t rank: %s\t use: %d%%\n",
				System.tasks[i].tname, status, System.tasks[i].tid, 
				priority, rank, 
				Top.processCpuUsage(System.tasks[i].tid));
		}
	}
	printf ("\n");
	
	_Sti();

	return 0;
	
}
