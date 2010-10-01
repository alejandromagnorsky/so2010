#ifndef _KLIB_H_
#define _KLIB_H_

#include "defs.h"
#include "task.h"

/************************************************
** Constants and definitions:
*************************************************/
#pragma pack(1)

#define LINEBUF_LEN 100

#define KERNEL_DEVICES 2
#define USER_DEVICES 10
#define NUM_TTYS 4

#define NUM_TASKS 10

#define QUANTUM 1

#define STACK_SIZE 1024
#define	MAX_TASK_NAME	20

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

typedef int (*program) (char*);

enum {
    DEVICE_START,
    DEVICE_CURRENT,
    DEVICE_END
};

enum {
    DEVICE_SCREEN = 0,
    DEVICE_KEYBOARD
};

struct task_t {
    int tid;
    int trank, tpriority;
    int tstatus;
    char tname[MAX_TASK_NAME];
    
    void* stack;
        
    struct {
        int devcode;
        int wpos;
        int rpos;
    } odevs[10];    
    
};

typedef struct task_t* task_t;

#define DEVICE_TTY(X) (DEVICE_KEYBOARD + (X) + 1)

enum {
    SYSTEM_CALL_READ = 0,
    SYSTEM_CALL_WRITE,
    SYSTEM_CALL_SEEKR,
    SYSTEM_CALL_SEEKW,
    SYSTEM_CALL_TELLR,
    SYSTEM_CALL_TELLW,
    SYSTEM_CALL_EXEC
};

struct driver_t {
    size_t (*write) (int devcode, void* from, int nbytes);
    size_t (*read)  (int devcode, void* to, int nbytes);
    size_t (*seekr) (int devcode, int offset, int from);
    size_t (*seekw) (int devcode, int offset, int from);
    size_t (*tellr) (int devcode);
    size_t (*tellw) (int devcode);
};

typedef struct driver_t* driver_t;

struct device_t {
    int  id;
    char name[11];
    
    driver_t driver;

    void* addr;     /* Starting address of the device's memory area */
    size_t size;    /* Size of the device's memory area */
    
    size_t wpos;    /* Current writing position */
    size_t rpos;	/* Current Reading position */
};

typedef struct device_t* device_t;
/*
struct stScreenDeviceData {
    int  id;
    char name[11];
*/
    //void* addr;     /* Starting address of the device's memory area */
    //size_t wpos;        /* Current writing position */
   // size_t rpos;	/* Current Reading position */
 //   size_t size;       /* Size of the device's memory area */
    
 //   char buffer[80 * 25 * 2];
//};

//typedef struct stScreenDeviceData* device_screen_t;

struct system_t {
    long int    ticks;      /* Tick count since system boot */
    
    int         ndevices;   /* Number of devices */
    device_t    device[KERNEL_DEVICES + USER_DEVICES + NUM_TTYS]; /* Device array */

    int         atty;       /* Currently active terminal device index. */
    
    task_t          task;	/* Running task */
    struct task_t   tasks[NUM_TASKS];
    
    void (*addTick) ();
    long int (*getTicks) ();
    
    size_t (*write) (int devcode, void* from, size_t nbytes);
    size_t (*read)  (int devcode, void* to, size_t nbytes);
    size_t (*seekr) (int devcode, int offset, int from);
    size_t (*seekw) (int devcode, int offset, int from);
    size_t (*tellr) (int devcode);
    size_t (*tellw) (int devcode);
    int    (*exec)  (int (*f) (char*), char* args);
};

typedef struct system_t* system_t;

/************************************************
** Kernel functions and macros library:
*************************************************/

/* System calls */
#define THROW_INT80 __asm__ __volatile__ ("int $0x80;")

#define MOVTO_AL(X) __asm__ __volatile__ ("movb %0, %%al;"    \
                                          :                   \
                                          :"g" (X)            \
                                          :"%al"              \
                                         );
                    
#define MOVFROM_AL(X) __asm__ __volatile__ ("movb %%al, %0;"  \
                                            :"=m" (X)         \
                                           );                

#define MOVTO_EAX(X) __asm__ __volatile__ ("movl %0, %%eax;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%eax"            \
                                          );

#define MOVFROM_EAX(X) __asm__ __volatile__ ("movl %%eax, %0;"  \
                                             :"=m" (X)          \
                                            ); 


#define MOVTO_EBX(X) __asm__ __volatile__ ("movl %0, %%ebx;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%ebx"            \
                                          );

#define MOVFROM_EBX(X) __asm__ __volatile__ ("movl %%ebx, %0;"  \
                                             :"=m" (X)          \
                                            ); 

#define MOVTO_ECX(X) __asm__ __volatile__ ("movl %0, %%ecx;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%ecx"            \
                                          );

#define MOVFROM_ECX(X) __asm__ __volatile__ ("movl %%ecx, %0;"  \
                                             :"=m" (X)          \
                                            ); 

#define MOVTO_EDX(X) __asm__ __volatile__ ("movl %0, %%edx;"  \
                                            :                 \
                                           :"g" (X)           \
                                           :"%edx"            \
                                          );

#define MOVFROM_EDX(X) __asm__ __volatile__ ("movl %%edx, %0;"  \
                                             :"=m" (X)          \
                                            );                                             

size_t _sys_write(int devcode, void* from, size_t nbytes);
size_t _sys_read(int devcode, void* to, size_t nbytes);
size_t _sys_seekr(int devcode, int offset, int from);
size_t _sys_seekw(int devcode, int offset, int from);
size_t _sys_tellr(int devcode);
size_t _sys_tellw(int devcode);
int _sys_exec(int (*f) (char*), char* args);



struct TaskNamespace {
    task_t (*new) (int tid, int trank, int tpriority);
    
    void (*setPriority) (task_t, int);
    void (*setRank)    (task_t, int);
    void (*setStatus)   (task_t, int);
    
    int (*getPriority) (task_t);
    int (*getRank)    (task_t);
    int (*getStatus)   (task_t);
    int (*getTID) (task_t task);
    void (*getNextTask) ();
    struct task_t (*getTaskById) (int);
    task_t* (*getCurrentTask) ();
};

void _task_saveState   (task_t);
void _task_loadState   (task_t);

void _task_setPriority (task_t, int);
void _task_setRank    (task_t, int);
void _task_setStatus   (task_t, int);

int _task_getPriority (task_t);
int _task_getRank    (task_t);
int _task_getStatus   (task_t);
int _task_getTID (task_t task);
void _task_getNextTask 	();
struct task_t _task_getTaskById (int tid);
task_t* _task_getCurrentTask();

#endif
