/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       ipc.h
**  Content:    header file for the IPC abstract interface.
*/

#ifndef __IPC_H__
#define __IPC_H__

#include "queue.h"
#include "message.h"

#include <sys/types.h>
#include <netinet/in.h>

enum {
    IPCSTAT_DISCONNECTED,
    IPCSTAT_CONNECTING,
    IPCSTAT_CONNECTED,
    IPCSTAT_PREPARING,
    IPCSTAT_SERVING,
    
    IPCERR_THREAD,
    
    IPCERR_SSOCKET,
    IPCERR_SBIND,
    IPCERR_SCONNECT,
    IPCERR_SLISTEN,

	IPCERR_FIFOFILEUSED,
	IPCERR_FIFOWRITING,
	IPCERR_FIFOREADING,
	
	IPCERR_MSGGETFAILED,
    IPCERR_INVALIDPRIORITY,
    IPCERR_MSGSNDFAILED,
    IPCERR_MSGRCVFAILED
};

union un_ipcdata_t {
    
    struct {
        int fd;
        struct sockaddr_in addr;
    } sdata;
    
    struct {
		int fdw;
        int fdr;
        char fifonamew[50];
        char fifonamer[50];        
    } fifodata;
    
    struct {
    	int id;
    } queuedata;
    
};

typedef union un_ipcdata_t* ipcdata_t;

struct st_ipc_t {
    int stop;           /* Put here so that the thread can be stopped */
    int errn;           /* The errno code of errors is stored here */
    int status;         /* Status is stored here. Do not write! */
    
    pthread_t thread;   /* Returned by pthread_create */
    int maxclts;        /* Max clients. Servers will use this */
    
    ipcdata_t ipcdata;
    queue_t inbox, outbox;
};

typedef struct st_ipc_t* ipc_t;

int sendMessage(ipc_t, message_t);
/* Queues a message for sending. */

message_t recvMessage(ipc_t);
/* Returns messages in order of arrival, or NULL if there are none */

#endif
