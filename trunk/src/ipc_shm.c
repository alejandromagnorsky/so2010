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

void shmDisconnect(ipc_t client){
	client->stop = 1;
	qdel(client->inbox);
	qdel(client->outbox);
	free(client->ipcdata);
	free(client);
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

void shmStopServe(ipc_t server){
	server->stop = 1;
	qdel(server->inbox);
	qdel(server->outbox);
	free(server->ipcdata);
	free(server);
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
	
	int sval;
	while(ipc->stop != 1){
		if(sem_trywait(clientWriteLock) != -1){
			shmHandlerServerReadMessage(ipc);
			sem_post(clientWriteLock);
		}
		if(sem_trywait(clientReadLock) != -1){
			shmHandlerWriteMessage(ipc);
			sem_post(clientReadLock);
		}
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
		if(sem_trywait(clientReadLock) != -1){
			shmHandlerReadMessage(ipc);
			sem_post(clientReadLock);
		}
		if(sem_trywait(clientWriteLock) != -1){
			shmHandlerWriteMessage(ipc);
			sem_post(clientWriteLock);
		}
	}
	sem_close(clientWriteLock);
	sem_close(clientReadLock);

	printf("Termina thread cliente.\n");
}


mheader_t nextHeaderMessage(ipc_t ipc){
	databuf_t buffer = ipc->ipcdata->shmdata.bufr;
	int i; int auxPtr = ((databuf_t) ipc->ipcdata->shmdata.bufr)->pread;
	char * nextHeader = NULL;
	int auxPtrRead = buffer->pread;
	if(canReadN(buffer, M_HEADER_SIZE) == VALIDPTR){
		nextHeader = malloc(M_HEADER_SIZE);
		for(i = 0; i < M_HEADER_SIZE; i++, nextBufPtr(&auxPtr)){
			nextHeader[i] = buffer->buffer[auxPtr];
		}
	}
	return (mheader_t) nextHeader;
}


void shmHandlerReadMessage(ipc_t ipc_client){
	mheader_t nextHeader = nextHeaderMessage(ipc_client);
	databuf_t buffer = ipc_client->ipcdata->shmdata.bufr;
	message_t incomingMsg;
	int i = 0;
	
	if(nextHeader != NULL && nextHeader->to == ipc_client->id && canReadN(buffer, M_HEADER_SIZE + nextHeader->len) == VALIDPTR){
		addBufPtr(&(buffer->pread), M_HEADER_SIZE);
		incomingMsg = mhnew(nextHeader, NULL);
		readBuf(buffer, incomingMsg->data, nextHeader->len);
		qput(ipc_client->inbox, incomingMsg);
		mdel(incomingMsg);
		free(nextHeader);
	}
}


void shmHandlerServerReadMessage(ipc_t ipc_server){
	mheader_t nextHeader = nextHeaderMessage(ipc_server);
	message_t incomingMsg;
	databuf_t buffer = ipc_server->ipcdata->shmdata.bufr;
	int i = 0;
	if(nextHeader != NULL && canReadN(buffer, M_HEADER_SIZE + nextHeader->len) == VALIDPTR){
		addBufPtr(&(buffer->pread), M_HEADER_SIZE);
		incomingMsg = mhnew(nextHeader, NULL);
		readBuf(buffer, incomingMsg->data, incomingMsg->header.len);
		qput(ipc_server->inbox, incomingMsg);
		free(nextHeader);
		mdel(incomingMsg);
	}
}


void shmHandlerWriteMessage(ipc_t ipc){
	message_t msgToSend = qpeek(ipc->outbox);
	char * aux;
	databuf_t buffer = ipc->ipcdata->shmdata.bufw;
	if(msgToSend != NULL){
		if(canWriteN(buffer, M_HEADER_SIZE + msgToSend->header.len) == VALIDPTR){
			mdel(msgToSend);
			msgToSend = qget(ipc->outbox);
			writeBuf(buffer, (aux = mserial(msgToSend)), M_HEADER_SIZE + msgToSend->header.len);
			free(aux);
			mdel(msgToSend);
		}
	}
}


int writeBuf(databuf_t databuf, char * buffer, int nwrite){
	int i;
	for(i = 0; i < nwrite; i++, nextBufPtr(&(databuf->pwrite))){
		databuf->buffer[databuf->pwrite] = buffer[i];
	}
	return nwrite;
}

int readBuf(databuf_t databuf, char * buffer, int nread){
	int i;
	for(i = 0; i < nread; i++, nextBufPtr(&(databuf->pread))){
		 buffer[i] = databuf->buffer[databuf->pread];
	}
	return nread;
}

void nextBufPtr(int * nptr){
	addBufPtr(nptr, 1);
}

void addBufPtr(int * nptr, int n){
	int aux = *(nptr) + n;
	if(aux >= BSIZE ){
		*(nptr) = aux % BSIZE; 
	}else{
		*(nptr) = aux;
	}
}


int canReadN(databuf_t databuf, int pread){
	int readptr = databuf->pread;
	int writeptr = databuf->pwrite;
	int afterread = (readptr + pread) % BSIZE;
	
	if(writeptr > readptr){
		if(pread <= (writeptr - readptr)){
			return VALIDPTR;
		}
	}else if(writeptr < readptr){
		if(pread <= (BSIZE - readptr + writeptr)){
			return VALIDPTR;
		}
	}
	return INVALIDPTR;
}

int canWriteN(databuf_t databuf, int pwrite){
	int readptr = databuf->pread;
	int writeptr = databuf->pwrite;
	int afterwrite = (writeptr + pwrite) % BSIZE;
	
	if(writeptr > readptr){
		if(pwrite >= (BSIZE - writeptr + readptr)){
			return INVALIDPTR;
		}
	}else if(writeptr < readptr){
		if(pwrite >= (readptr - writeptr)){
			return INVALIDPTR;
		}
	}
	return VALIDPTR;
}