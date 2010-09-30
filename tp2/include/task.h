/*
** Task header file
*/

#ifndef __TASK_H__
#define __TASK_H__

#include "defs.h"


enum {
    RANK_SERVER,
    RANK_NORMAL
};

enum {
    PRIORITY_LOW,
    PRIORITY_MEDIUM,
    PRIORITY_HIGH,
    PRIORITY_MAX
};

enum {
    STATUS_READY,
    STATUS_WAITING,
    STATUS_DEAD,
    STATUS_RUNNING /* ? */
};

struct task_t {
    int tid;
    int trank, tpriority;
    int tstatus;
    
    int dtyIndex;
    
    void* stack;
        
    struct {
        int devcode;
        int wpos;
        int rpos;
    } odevs[10];    
    
};

typedef struct task_t* task_t;

struct TaskNamespace {
    task_t (*new) (int tid, int trank, int tpriority);
    void (*loadState)   (task_t);
    void (*saveState)   (task_t);
    
    void (*setPriority) (task_t, int);
    void (*setRank)    (task_t, int);
    void (*setStatus)   (task_t, int);
    
    int (*getPriority) (task_t);
    int (*getRank)    (task_t);
    int (*getStatus)   (task_t);
};

void _task_saveState   (task_t);
void _task_loadState   (task_t);

void _task_setPriority (task_t, int);
void _task_setRank    (task_t, int);
void _task_setStatus   (task_t, int);

int _task_getPriority (task_t);
int _task_getRank    (task_t);
int _task_getStatus   (task_t);

#endif
