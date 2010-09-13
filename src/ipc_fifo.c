#include "../include/ipc_fifo.h"

int initFifos(int qtyAnts){
	if(qtyAnts <= 0)
		return;	

	char word[20];
	int i = FIRST_ANT_ID;
	
	while(i < qtyAnts + FIRST_ANT_ID){
		sprintf(word, "/tmp/fifo_c_w_%d", i);
		if(mkfifo(word, PERMISSIONS) < 0){
			return -1;
		}
		sprintf(word, "/tmp/fifo_c_r_%d", i);
		if(mkfifo(word, PERMISSIONS) < 0){
			return -1;
		}
		i++;
	}
}


ipcdata_t fifoIPCData(int antid){
	ipcdata_t ansdata = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
	if(ansdata != NULL){
		sprintf((ansdata->fifodata).fifonamew, "/tmp/fifo_c_w_%d", antid);
		sprintf((ansdata->fifodata).fifonamer, "/tmp/fifo_c_r_%d", antid);
	}
	return ansdata;
}

ipc_t fifoConnect(ipcdata_t ipcdata, int antid){
		
	ipc_t ret = (ipc_t) malloc(sizeof(struct st_ipc_t));
	int rcreat = 0;
	if(ipcdata == NULL){
		ret->status = IPCSTAT_DISCONNECTED;
		return ret;
	}
	
	if(ret != NULL){
		ret->status = IPCSTAT_CONNECTING;
		ret->ipcdata = ipcdata;
		ret->inbox = qnew();
		ret->outbox = qnew();
		ret->id = antid;

		ret->ipcdata->fifodata.fdw = open(ret->ipcdata->fifodata.fifonamew, O_RDWR | O_NONBLOCK);
		ret->ipcdata->fifodata.fdr = open(ret->ipcdata->fifodata.fifonamer, O_RDONLY | O_NONBLOCK);

		if(ret->ipcdata->fifodata.fdw < 0 || ret->ipcdata->fifodata.fdr < 0){
			ret->status = IPCERR_OPENFIFO;
		}else{
			ret->status = IPCSTAT_CONNECTED;
			rcreat = pthread_create(&(ret->thread), NULL, fifoClientLoop, ret);	
			if(rcreat != 0){
				ret->status = IPCERR_THREAD;
				return ret;
			}
		}
	}
	return ret;
}


ipc_t fifoServe(int nclients){

	initFifos(nclients);

	int rcreat;
    ipc_t ret = (ipc_t) calloc (1, sizeof(struct st_ipc_t));
    
    ret->status = IPCSTAT_PREPARING;
    ret->maxclts = nclients;
    ret->ipcdata = NULL;
    ret->inbox = qnew();
    ret->outbox = qnew();

    if((ret->thread = pthread_create(&(ret->thread), NULL, fifoServerLoop, ret)) != 0);
        ret->status = IPCERR_THREAD;

   return ret;
}


int fifoDisconnect(ipc_t ipc){
	ipc->stop = 1;
	while(ipc->status != IPCSTAT_DISCONNECTED){
		//FINALIZING THE THREAD.
	}
	free(ipc->ipcdata);
	qdel(ipc->inbox);
	qdel(ipc->outbox);
	free(ipc);
}


void fifoHandlerWrite(ipc_t ipc, int fdw, msg_writting currMsgW){
	message_t nextMsg;
	int offset = 0;
	int nwrite = 0;
	if(currMsgW->toWrite == 0){
		nextMsg = qget(ipc->outbox);
		if(nextMsg != NULL){
			currMsgW->toWrite = M_HEADER_SIZE + nextMsg->header.len;
			currMsgW->msglen = currMsgW->toWrite;
			currMsgW->data = mserial(nextMsg);
			currMsgW->to = nextMsg->header.to;
			currMsgW->from = nextMsg->header.from;
			mdel(nextMsg);
		}
	}else{
		offset = currMsgW->msglen - currMsgW->toWrite;
		nwrite = write(fdw, currMsgW->data, currMsgW->toWrite);
		if(nwrite < 0){
			ipc->status = IPCERR_MSGSNDFAILED;
			return;
		}
		if((currMsgW->toWrite -= nwrite) == 0){
			free(currMsgW->data);
		}
	}
}

void fifoHandlerWriteServer(ipc_t ipc, msg_writting currMsgW, client_t * clients){
	message_t nextMsg;
	if(currMsgW->toWrite == 0){
		nextMsg = qget(ipc->outbox);
		if(nextMsg != NULL){
			currMsgW->toWrite = M_HEADER_SIZE + nextMsg->header.len;
			currMsgW->msglen = currMsgW->toWrite;
			currMsgW->data = mserial(nextMsg);
			currMsgW->to = nextMsg->header.to;
			currMsgW->from = nextMsg->header.from;
			mdel(nextMsg);
		}
	}else{
		fifoHandlerWrite(ipc, clients[currMsgW->to - FIRST_ANT_ID]->cinfo->fifodata.fdw, currMsgW);
	}
}

void fifoHandlerRead(ipc_t ipc, int fdr, queue_t inbox, msg_reading currMsgR){
	int offset = 0; int nread;
	if(currMsgR->toRead == 0){
		nread = read(fdr, currMsgR->bufferhd, M_HEADER_SIZE - currMsgR->hdread);
		if(nread < 0){
			currMsgR->hdread = 0;
			IPCERR_MSGGETFAILED;
			return;
		}
		currMsgR->hdread += nread;
		if(currMsgR->hdread == M_HEADER_SIZE){
			currMsgR->incomingMsg = mhnew( (mheader_t) currMsgR->bufferhd, NULL);
			currMsgR->toRead = currMsgR->incomingMsg->header.len;
			currMsgR->hdread = 0;
		}
	}else{
		offset = currMsgR->incomingMsg->header.len - currMsgR->toRead;
		nread = read(fdr, currMsgR->incomingMsg->data + offset, currMsgR->toRead);
		if(nread < 0){
			currMsgR->hdread = 0;
			IPCERR_MSGGETFAILED;
			return;
		}
		currMsgR->toRead -= nread;
		if(currMsgR->toRead == 0){
			qput(inbox, currMsgR->incomingMsg);
			mdel(currMsgR->incomingMsg);
		}  
	}
}


void* fifoClientLoop(void* ipcarg){
	ipc_t ipc = (ipc_t) ipcarg;
	
	msg_writting currMsgW = (msg_writting) malloc(sizeof(struct st_msg_writting));
	msg_reading currMsgR = (msg_reading) malloc(sizeof(struct st_msg_reading));
	currMsgR->incomingMsg = (message_t) malloc(sizeof(struct st_message_t));
	
	currMsgW->toWrite = currMsgR->toRead = currMsgR->hdread = 0;
	
	while(ipc->stop != 1){
		fifoHandlerWrite(ipc, ipc->ipcdata->fifodata.fdw, currMsgW);
		fifoHandlerRead(ipc, ipc->ipcdata->fifodata.fdr, ipc->inbox, currMsgR);
	}
	
	free(currMsgR->incomingMsg);
	free(currMsgR);
	free(currMsgW);
	ipc->status = IPCSTAT_DISCONNECTED;
}


void* fifoServerLoop(void* ipcarg){
	ipc_t ipc = (ipc_t) ipcarg;
	fd_set readfds;
	int nclt;
	
	client_t * clients = calloc(ipc->maxclts, sizeof(client_t));
	client_t currClient;
	
	msg_writting currMsgW = (msg_writting) malloc(sizeof(struct st_msg_writting));
	currMsgW->toWrite = 0;
	
	initializeClientsServer(ipc, clients);
	if(ipc->status == IPCSTAT_DISCONNECTED){
		return;
	}
	
	ipc->status = IPCSTAT_SERVING;
	
	while(ipc->stop != 1){
		FD_ZERO(&readfds);
		for(nclt = 0; nclt < ipc->maxclts; nclt++){
			FD_SET(clients[nclt]->cinfo->fifodata.fdr, &readfds);
		}
		select(FD_SETSIZE, &readfds, NULL, NULL, NULL);
		for(nclt = 0; nclt < ipc->maxclts; nclt++){
			if(FD_ISSET(clients[nclt]->cinfo->fifodata.fdr, &readfds)){
				currClient = clients[nclt];
				fifoHandlerRead(ipc, clients[nclt]->cinfo->fifodata.fdr, ipc->inbox, clients[nclt]->currMsgR);
			}
		}
		fifoHandlerWriteServer(ipc, currMsgW, clients);
	}
	finalizeClientsServer(ipc, clients);
	ipc->status = IPCSTAT_DISCONNECTED;
	free(currMsgW);
}

void initializeClientsServer(ipc_t ipc, client_t * clients){
	int nclt;
	for(nclt = 0; nclt < ipc->maxclts; nclt++){
			clients[nclt] = (client_t) malloc(sizeof(struct st_client_t));
			clients[nclt]->cinfo = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
			clients[nclt]->currMsgR = (msg_reading) malloc(sizeof(struct st_msg_reading));
			clients[nclt]->currMsgR->hdread = clients[nclt]->currMsgR->toRead = 0; 
			sprintf(clients[nclt]->cinfo->fifodata.fifonamew, "/tmp/fifo_c_r_%d", nclt + FIRST_ANT_ID);
			sprintf(clients[nclt]->cinfo->fifodata.fifonamer, "/tmp/fifo_c_w_%d", nclt + FIRST_ANT_ID);
			clients[nclt]->cinfo->fifodata.fdw = open(clients[nclt]->cinfo->fifodata.fifonamew, O_RDWR | O_NONBLOCK);
			clients[nclt]->cinfo->fifodata.fdr = open(clients[nclt]->cinfo->fifodata.fifonamer, O_RDONLY | O_NONBLOCK);
			if(clients[nclt]->cinfo->fifodata.fdw < 0 || clients[nclt]->cinfo->fifodata.fdr < 0){
				printf("Error: opening clients.\n");
				ipc->status = IPCSTAT_DISCONNECTED;
				return;
			}
		}
}

void finalizeClientsServer(ipc_t ipc, client_t * clients){
	int nclt;
	for(nclt = 0; nclt < ipc->maxclts; nclt++){
			unlink(clients[nclt]->cinfo->fifodata.fifonamer);
			unlink(clients[nclt]->cinfo->fifodata.fifonamew);
			free(clients[nclt]->cinfo);
			free(clients[nclt]->currMsgR);
			free(clients[nclt]);
		}
	free(clients);
}

void stopServer(ipc_t ipc){
	ipc->stop = 1;
	while(ipc->status != IPCSTAT_DISCONNECTED){
		//FINALIZING THE THREAD.
	}
	qdel(ipc->inbox);
	qdel(ipc->outbox);
	free(ipc);
	printf("Termino server.\n");
}