#ifndef _KLIB_H_
#define _KLIB_H_

/************************************************
** Constants and definitions:
*************************************************/
#pragma pack(1)
#define LINEBUF_LEN 100
#define NULL 0

typedef int (*program) (char*);
typedef int size_t;
typedef short int ssize_t;

enum {
    DEVICE_START,
    DEVICE_CURRENT,
    DEVICE_END
};

enum {
    DEVICE_SCREEN = 0,
    DEVICE_KEYBOARD
};

enum {
    SYSTEM_CALL_READ = 0,
    SYSTEM_CALL_WRITE,
    SYSTEM_CALL_SEEKR,
    SYSTEM_CALL_SEEKW,
    SYSTEM_CALL_TELLR,
    SYSTEM_CALL_TELLW,
    SYSTEM_CALL_EXEC
};

struct stDeviceData {
    int  id;
    char name[11];

    void* addr;     /* Starting address of the device's memory area */
    size_t wpos;        /* Current writing position */
    size_t rpos;	/* Current Reading position */
    size_t size;       /* Size of the device's memory area */
};

typedef struct stDeviceData* device_t;
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


struct stSystemData {
    long int    ticks;      /* Tick count since system boot */
    
    int         ndevices;   /* Number of devices */
    device_t    device[10]; /* Device array */

    int         atty;       /* Currently active terminal. Not implemented. */
};

typedef struct stSystemData* system_t;

/************************************************
** Kernel functions and macros library:
*************************************************/

/* System data structure manipulation: */
void _saddTick(system_t sys);
long int _sgetTicks(system_t sys);

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

size_t write(int devcode, void* from, int nbytes);
size_t read(int devcode, void* to, int nbytes);
size_t seekr(int devcode, int offset, int from);
size_t seekw(int devcode, int offset, int from);
size_t tellr(int devcode);
size_t tellw(int devcode);
#endif
