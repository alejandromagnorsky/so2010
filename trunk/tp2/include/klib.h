#ifndef _KLIB_H_
#define _KLIB_H_

#include "defs.h"

/************************************************
** Constants and definitions:
*************************************************/
#pragma pack(1)

#define KERNEL_DEVICES 2
#define USER_DEVICES 10
#define NUM_TTYS 4

#define NUM_TASKS 20

#define QUANTUM 2

#define STACK_SIZE 1024
#define	MAX_TASK_NAME	20

#define PAGE_SIZE 0x1000
#define DEFAULT_STACK_SIZE 0x1000

#define LASTS_QUANT 100

#define CANT_PRIORITY 4
#define RATIO 2


enum {
    RANK_SERVER,
    RANK_NORMAL
};

enum {
    PRIORITY_MAX,
    PRIORITY_HIGH,
    PRIORITY_MEDIUM,
    PRIORITY_LOW,
    PRIORITY_MIN
};

enum {
    STATUS_READY,
    STATUS_WAITING,
    STATUS_WAITING_SLEEP,
    STATUS_WAITING_RECV,
    STATUS_WAITING_SEND,
    STATUS_DEAD,
    STATUS_RUNNING,
};

typedef int (*program_t) (char*);

enum {
    DEVICE_START,
    DEVICE_CURRENT,
    DEVICE_END
};

enum {
    DEVICE_SCREEN = 0,
    DEVICE_KEYBOARD,
	DEVICE_TTY
};


enum {
	RUNNING_FRONT = 0,
	RUNNING_BACK,
};

struct block_t {
	struct block_t * next;
	void * firstPage;
	unsigned int npages;
	unsigned int freeSpace;
	void * ptrFreeMemory;
};

typedef struct block_t * block_t;

struct task_t {
    int tid;
    char tname[MAX_TASK_NAME + 1];
    int trank, tpriority;
    int tstatus;
    
    union {
        
        struct {
            int tid, len;
            char msg[128];
        } send;

        struct {
            int tid, len;
            char msg[128];
        } recv;

        struct {
            int ticks;
        } sleep;
        
    } tsdata; /* Status additional information */
    
    int isFront;
        
    void* stack;		/* Memory direction where stack starts */
    void* stack_start;	/* Begin of stack for the program (grows downwards) */
    int stack_size;		/* Stack size */
    int esp;			/* Stack pointer */
    
    int parentTID;

	int tty;			/* TTY where is running */
	char running_mode;	/* RUNNING_BACK || RUNNING_FRONT */

	block_t mem;

	// [TODO] we are not using this:
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
    SYSTEM_CALL_EXEC,
    SYSTEM_CALL_GETTID,
    SYSTEM_CALL_NEXTTID,
    SYSTEM_CALL_GETRANK,
    SYSTEM_CALL_GETPRIO,
    SYSTEM_CALL_GETCPUC,
    SYSTEM_CALL_NAME,
    SYSTEM_CALL_SLEEP,
    SYSTEM_CALL_RECV,
    SYSTEM_CALL_SEND,
    SYSTEM_CALL_GETMSG,
    SYSTEM_CALL_CLSMSG,
    SYSTEM_CALL_YIELD,
    SYSTEM_CALL_KILL
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


struct system_t {
    long int    ticks;      /* Tick count since system boot */
    
    int         ndevices;   /* Number of devices */
    device_t    device[KERNEL_DEVICES + USER_DEVICES + NUM_TTYS]; /* Device array */

    int         atty;       /* Currently active terminal device index. */

    task_t		idle;	              /* Idle task */
    task_t		task;	              /* Running task */
    struct task_t   tasks[NUM_TASKS]; /* Static task array */
    
    int			ttysTids[NUM_TTYS];		/* ttys tids */
    
    int			last100[LASTS_QUANT];	/* For top */
    int			last100Counter;
    
    void (*addTick) ();
    long int (*getTicks) ();
    
    size_t (*write) (int devcode, void* from, size_t nbytes);
    size_t (*read)  (int devcode, void* to, size_t nbytes);
    size_t (*seekr) (int devcode, int offset, int from);
    size_t (*seekw) (int devcode, int offset, int from);
    size_t (*tellr) (int devcode);
    size_t (*tellw) (int devcode);
    void* (*malloc) (size_t size);
    void (*free) ();
    int    (*exec)  (int (*f) (char*), char* args);
    int (*gettid) ();
    int (*nexttid) (int*);
    int (*getprio) (int);
    int (*getrank) (int);
    int (*getcpuc) (int);
    char* (*name) (char*);
    int (*sleep) (int);
    int (*send) (int, void*, int);
    int (*recv) ();
    int (*getmsg) (void*, int);
    int (*clsmsg) ();
    int (*yield) ();
    int (*kill) (int);
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

int _sys_gettid(char* name);
int _sys_nexttid(int* iter);
int _sys_getrank(int pid);
int _sys_getprio(int pid);
int _sys_getcpuc(int pid);
char* _sys_name(char* name);

int _sys_send(int to, void* buf, int len);
int _sys_recv();
int _sys_getmsg(void* buf, int len);
int _sys_clsmsg();

int _sys_sleep(int);
int _sys_yield();
int _sys_kill(int tid);

struct TaskNamespace {    
    void (*setPriority) (task_t, int);
    void (*setRank)    (task_t, int);
    void (*setStatus)   (task_t, int);
    
    int (*getPriority) (task_t);
    int (*getRank)    (task_t);
    int (*getStatus)   (task_t);
    int (*getTID) (task_t task);
    
    int (*findSlot) ();
    int (*new) (task_t, char*, program_t, int, int, int, int, char*);
    void (*kill) (int);

    task_t (*getByTID) (int);
    task_t (*getCurrent) ();
    task_t (*getByName) (char*);

    int (*getNewTID) ();
    void (*setupScheduler) ();
    int (*scheduler) (int);
    void (*cleaner) (void);

	void (*setTty)(task_t, int);
	int (*getTty)(task_t);
	void (*runInBackground)(task_t);
	int (*getRunningMode)(task_t);
	void (*setParentTID)(task_t, int);
	int (*getParentTID)(task_t);
	void (*yield)(int);
	int (*checkTTY)(int taskTID);
	void (*setSleep)(task_t task, int ticks);
	int (*decSleep)(task_t);
	int (*getSleep)(task_t);
    void (*maintenance) ();
    int (*setSend) (task_t, int, void*, int);
    int (*setRecv) (task_t);
};

struct TopNamespace {
	int (*increment100Counter)();
	int (*processCpuUsage)(int);
	void (*getStatusName)(char*, task_t);
	void (*getRankName)(char*, task_t);
	void (*getPriority)(char*, task_t);
	void (*initialize)(int);
	void (*clearTask)(int);
	int (*run)();
};

void _task_saveState   (task_t);
void _task_loadState   (task_t);

void _task_setPriority (task_t, int);
void _task_setRank    (task_t, int);
void _task_setStatus   (task_t, int);
int _task_setESP (task_t task, int);

int _task_getPriority (task_t);
int _task_getRank    (task_t);
int _task_getStatus   (task_t);
int _task_getTID (task_t task);
int _task_getESP (task_t task);
int _task_findSlot();

void _task_setTty(task_t, int);
int _task_getTty(task_t);
void _task_runInBackground(task_t);
int _task_getRunningMode(task_t);
void _task_setParentTID(task_t task, int parentTID);
int _task_getParentTID(task_t task);

int _task_new (task_t task, char* name, program_t program, int rank, 
			int priority, int isFront, int tty, char* line);
void _task_kill(int tid);

task_t _task_getCurrent();
task_t _task_getByTID (int tid);
task_t _task_getByName(char*);

int _task_getNewTID();
static void _task_cleaner (void);
void _task_setupScheduler ();
int _task_scheduler(int esp);
int Idle (void);
void _task_yield(int tid);
int _task_checkTTY(int taskTID);

void _task_maintenance();

void _task_setSleep(task_t task, int ticks);
int _task_decSleep(task_t task);
int _task_getSleep(task_t task);

int _task_setSend(task_t, int to, void* buf, int len);
int _task_setRecv(task_t);

int _top_increment100Counter();
int _top_processCpuUsage(int tid);
void _top_getStatusName(char* buffer, task_t task);
void _top_getRankName(char* buffer, task_t task);
void _top_getPriority(char* buffer, task_t task);
void _top_initialize(int tid);
void _top_clearTask(int tid);
int _top_run();

extern struct PagingNamespace Paging;
extern tty_t ttys[NTTYS];

#endif
