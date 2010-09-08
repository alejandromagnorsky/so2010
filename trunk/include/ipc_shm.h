#ifndef IPC_FIFO_H_
#define IPC_FIFO_H_

#include "ipc.h"

#include <stdio.h>
#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <semaphore.h>

#define	BSIZE	5 * BUFSIZ

#define INVALIDPTR	-1
#define VALIDPTR	0

#define QTYSEM 2

#define WRITELOCK "/writelock1"
#define READLOCK "/writelock2"


#define ERRSEM	((sem_t *) -1)

struct st_databuf_t {
	int pread;
	int pwrite;
	char buffer[BSIZE];
};
typedef struct st_databuf_t * databuf_t; 

typedef union _semun {
	int val;
	struct semid_ds *buf;
	ushort *array;
} semun;

#define ERR ((char *) -1)


ipcdata_t shmIPCData();
/* Generates an ipcdata_t for Shared Memory IPC */

ipc_t shmConnect(ipcdata_t ipcdata, int antid);
/* Fires a thread connected with the server */

ipc_t shmServe();
/* Starts the server that allows clients read & write messages */
ipc_t shmServeFail(ipc_t ret, int error);

void* shmServerLoop(void * ipcarg);
/* Server loop */

void* shmCientLoop(void * ipcarg);
/* Client loop */


mheader_t nextHeaderMessage(ipc_t ipc_client);

void nextBufPtr(int * nptr);

void shmHandlerReadMessage(ipc_t ipc_client);

void shmHandlerServerReadMessage(ipc_t ipc_server);

int readBuf(databuf_t databuf, char * buffer, int nread);

void addBufPtr(int * nptr, int n);

int canWriteN(databuf_t databuf, int pwrite);
int canReadN(databuf_t databuf, int pread);

void shmHandlerWriteMessage(ipc_t ipc);

#endif