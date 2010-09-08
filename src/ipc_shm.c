#include "../include/ipc_shm.h"

static int shmidw, shmidr, semid;


ipcdata_t shmIPCData(){
	
	ipcdata_t ret = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
	
	ret->shmdata.bufw = (databuf_t) shmat(shmidw, 0, 0);
	ret->shmdata.bufr = (databuf_t) shmat(shmidr, 0, 0);
	if((ret->shmdata.bufw == ERR) || (ret->shmdata.bufr == ERR)){
		free(ret);
		return NULL;
	}
	return ret;
}

ipc_t shmConnect(ipcdata_t ipcdata, int antid){
	ipc_t ret = (ipc_t) malloc(sizeof(struct st_ipc_t));
	int rcreat = 0;
	
	if(ret != NULL){
		if(ipcdata == NULL){
			ret->status = IPCSTAT_DISCONNECTED;
			return ret;
		}
		ret->id = antid;
		ret->status = IPCSTAT_CONNECTING;
		ret->ipcdata = ipcdata;
		ret->inbox = qnew();
		ret->outbox = qnew();
		
		rcreat = pthread_create(&(ret->thread), NULL, shmCientLoop, ret);	
		if(rcreat != 0){
			ret->status = IPCERR_THREAD;
			qdel(ret->inbox);
			qdel(ret->outbox);
			free(ret->ipcdata);
			return ret;
		}
	}
	return ret;
}


ipc_t shmServe(){
	int semun, x;

	ipc_t ret = (ipc_t) calloc (1, sizeof(struct st_ipc_t));
	    
	ret->status = IPCSTAT_PREPARING;
	ret->ipcdata = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
	ret->inbox = qnew();
	ret->outbox = qnew();

	/* create shared memory segment */
	shmidw = shmget(IPC_PRIVATE, sizeof(struct st_databuf_t), 0644 | IPC_CREAT);
	shmidr = shmget(IPC_PRIVATE, sizeof(struct st_databuf_t), 0644 | IPC_CREAT);
	if(shmidw == -1 || shmidr == -1){
		return shmServeFail(ret, IPCERR_SHMSEM);
	}
	
	/* attach shared memory segments */
	ret->ipcdata->shmdata.bufw = (databuf_t) shmat(shmidr, 0, 0);
	ret->ipcdata->shmdata.bufr = (databuf_t) shmat(shmidw, 0, 0);
	if((ret->ipcdata->shmdata.bufw == ERR) || (ret->ipcdata->shmdata.bufr == ERR)) {
		return shmServeFail(ret, IPCERR_SHMATTACH);
	}
	
	/* Start server loop in a thread */
	if((ret->thread = pthread_create(&(ret->thread), NULL, shmServerLoop, ret)) != 0);
		ret->status = IPCERR_THREAD;

	return ret;
}


ipc_t shmServeFail(ipc_t ret, int error){
	free(ret->ipcdata);
	qdel(ret->inbox);
	qdel(ret->outbox);
	ret->status = error;
	return ret;
}

void* shmServerLoop(void * ipcarg){
	/* create locks */
	sem_t * clientReadLock = sem_open(READLOCK, O_CREAT, 0666, 1);
	sem_t * clientWriteLock = sem_open(WRITELOCK, O_CREAT, 0666, 1);

	if( clientReadLock == ERRSEM || clientWriteLock == ERRSEM ){
		return;
	}
	
	sem_init(clientReadLock, 1, 1);
	sem_init(clientWriteLock, 1, 1);
	
	ipc_t ipc = (ipc_t) ipcarg;
	ipc->status = IPCSTAT_SERVING;
	while(ipc->stop != 1){
//		if(sem_trywait(clientWriteLock) == -1){
			shmHandlerServerReadMessage(ipc);
//			sem_post(clientWriteLock);
//		}
//		if(sem_trywait(clientReadLock) == -1){
			shmHandlerWriteMessage(ipc);
//			sem_post(clientReadLock);
//		}
	}
	
	sem_close(clientReadLock);
	sem_close(clientWriteLock);
	sem_unlink(READLOCK);
	sem_unlink(WRITELOCK);
	printf("Termina thread server.\n");
}

void* shmCientLoop(void * ipcarg){
	sem_t * clientReadLock = sem_open(READLOCK, 0);
	sem_t * clientWriteLock = sem_open(WRITELOCK, 0);
	
	if( clientReadLock == ERRSEM || clientWriteLock == ERRSEM ){
		return;
	}

	ipc_t ipc = (ipc_t) ipcarg;
	ipc->status = IPCSTAT_CONNECTED;
	while(ipc->stop != 1){
		if(sem_trywait(clientWriteLock) == -1){
			shmHandlerWriteMessage(ipc);
			sem_post(clientWriteLock);
		}
		if(sem_trywait(clientReadLock) == -1){
			shmHandlerReadMessage(ipc);
			sem_post(clientReadLock);
		}
	}
	sem_close(clientWriteLock);
	sem_close(clientReadLock);

	printf("Termina thread cliente.\n");
}


mheader_t nextHeaderMessage(ipc_t ipc){
	databuf_t buffer = ipc->ipcdata->shmdata.bufr;
	char * nextHeader = NULL;
	int auxPtrRead = buffer->pread;
	if(canReadN(buffer, M_HEADER_SIZE) == VALIDPTR){
		nextHeader = malloc(M_HEADER_SIZE);
		memcpy(nextHeader, buffer->buffer + auxPtrRead, M_HEADER_SIZE);
	}
	return (mheader_t) nextHeader;
}


void shmHandlerReadMessage(ipc_t ipc_client){
	mheader_t nextHeader = nextHeaderMessage(ipc_client);
	databuf_t buffer = ipc_client->ipcdata->shmdata.bufr;
	message_t incomingMsg;
	int i = 0;
	
	if(nextHeader != NULL && nextHeader->to == ipc_client->id){
		addBufPtr(&(buffer->pread), M_HEADER_SIZE);
		if(canReadN(buffer, nextHeader->len - 1) == VALIDPTR){
			incomingMsg = mhnew(nextHeader, NULL);
			readBuf(buffer, incomingMsg->data, nextHeader->len);
			qput(ipc_client->inbox, incomingMsg);
			mdel(incomingMsg);
			free(nextHeader);
		}else{
			ipc_client->status = IPCERR_MSGGETFAILED;
			perror("Malformed message.");
			exit(1);
		}
	}
}


void shmHandlerServerReadMessage(ipc_t ipc_server){
	mheader_t nextHeader = nextHeaderMessage(ipc_server);
	message_t incomingMsg;
	databuf_t buffer = ipc_server->ipcdata->shmdata.bufr;
	int i = 0;
	if(nextHeader != NULL){
		if(canReadN(buffer, M_HEADER_SIZE + nextHeader->len) == VALIDPTR){
			addBufPtr(&(buffer->pread), M_HEADER_SIZE);
			incomingMsg = mhnew(nextHeader, NULL);
			readBuf(buffer, incomingMsg->data, incomingMsg->header.len);
			qput(ipc_server->inbox, incomingMsg);
			free(nextHeader);
			mdel(incomingMsg);
		}
	}
}


void shmHandlerWriteMessage(ipc_t ipc){
	message_t msgToSend = qpeek(ipc->outbox);
	char * aux;
	databuf_t buffer = ipc->ipcdata->shmdata.bufw;
	if(msgToSend != NULL){
		if(canWriteN(buffer, msgToSend->header.len) == VALIDPTR){
			mdel(msgToSend);
			msgToSend = qget(ipc->outbox);
			writeBuf(buffer, (aux = mserial(msgToSend)), M_HEADER_SIZE + msgToSend->header.len);
			free(aux);
			mdel(msgToSend);
		}
	}
}


int writeBuf(databuf_t databuf, char * buffer, int nwrite){
	int i = 0;
	for(i = 0; i < nwrite; i++){
		databuf->buffer[databuf->pwrite] = buffer[i];
		nextBufPtr(&(databuf->pwrite));
	}
	return nwrite;
}

int readBuf(databuf_t databuf, char * buffer, int nread){
	int i = 0;
	for(i = 0; i < nread; i++){
		 buffer[i] = databuf->buffer[databuf->pread];
		 nextBufPtr(&(databuf->pread));
	}
	return nread;
}

void nextBufPtr(int * nptr){
	addBufPtr(nptr, 1);
}

void addBufPtr(int * nptr, int n){
	int aux = *(nptr) + n;
	if(aux >= BUFSIZ ){
		*(nptr) = aux % BUFSIZ; 
	}else{
		*(nptr) = aux;
	}
}


int canReadN(databuf_t databuf, int pread){
	int readptr = databuf->pread;
	int writeptr = databuf->pwrite;
	int afterread = (readptr + pread) % BUFSIZ;
	
	if(writeptr > readptr){
		if(pread <= (writeptr - readptr)){
			return VALIDPTR;
		}
	}else if(writeptr < readptr){
		if(pread <= (BUFSIZ - readptr + writeptr)){
			return VALIDPTR;
		}
	}
	return INVALIDPTR;
}

int canWriteN(databuf_t databuf, int pwrite){
	int readptr = databuf->pread;
	int writeptr = databuf->pwrite;
	int afterwrite = (writeptr + pwrite) % BUFSIZ;
	
	if(writeptr > readptr){
		if(pwrite >= (BUFSIZ - writeptr + readptr)){
			return INVALIDPTR;
		}
	}else if(writeptr < readptr){
		if(pwrite >= (readptr - writeptr)){
			return INVALIDPTR;
		}
	}
	return VALIDPTR;
}