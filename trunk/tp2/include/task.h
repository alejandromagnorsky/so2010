/*
** Task header file
*/

#ifndef __TASK_H__
#define __TASK_H__

enum {
    RANK_SERVER,
    RANK_NORMAL
}

enum {
    PRIORITY_MAX,
    PRIORITY_HIGH,
    PRIORITY_MEDIUM,
    PRIORITY_LOW
}

struct task_t {
    int tid;
    int tclass, tpriority;
    
    struct {
        int eax, ebx, ecx, edx,
            sp, cs, ds, ip;
    } regs;
        
    struct {
        int devcode;
        size_t wpos;
        size_t rpos;
    } odevs[10];
    
    
}

typedef struct task_t* task_t;

#endif
