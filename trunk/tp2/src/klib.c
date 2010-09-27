#include "../include/klib.h"

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
    _task_getNextTask,
    _task_getTaskById
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

int _task_getStatus (task_t task) {
    return task->tstatus;
}

void _task_getNextTask() {
	
	task_t oldTask; /* obtener la tarea que se esta corriendo */
	task_t newTask; /* obtener la nueva tarea segun el scheduler */
	
	//if(oldTask->tid != newTask->tid)
	//{
		/* bajar las pages de la vieja task */
		//_task_save_state_();
		//_task_load_state_(newTask->stack);
		
		/* subir las pages de la nueva task */
//	}
	
	return;
}

task_t _task_getTaskById(int tid) {
	int i;
	
	//if(tid == 0)
	//{
		/* return idle process */
	//}
	
	/*for( i = 0; i < NUM_TASKS; i++)
	{
		if(system_t->tasks[i]->tid == tid)
		{
			return system_t->tasks[i];
		}
	}*/

}
