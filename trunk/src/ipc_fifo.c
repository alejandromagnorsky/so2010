#include "../include/ipc_fifo.h"


int initFifos(int qtyAnts){
	if(qtyAnts <= 0)
		return;	

	char word[20];
	int i = 0;
	
	while(i < qtyAnts){
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

void unlinkFifos(int qtyAnts){
	if(qtyAnts <= 0)
		return;	

	char word[20];
	int i = 0;
	
	while(i < qtyAnts){
		sprintf(word, "/tmp/fifo_c_w_%d", i);
		unlink(word);		
		sprintf(word, "/tmp/fifo_c_r_%d", i);
		unlink(word);		
		i++;
	}
}


ipcdata_t fifoIPCData(int nant){
	ipcdata_t ansdata = (ipcdata_t) malloc(sizeof(union un_ipcdata_t));
	if(ansdata != NULL){
		sprintf((ansdata->fifodata).fifonamew, "/tmp/fifo_c_w_%d", nant);
		sprintf((ansdata->fifodata).fifonamer, "/tmp/fifo_c_r_%d", nant);
	}
	return ansdata;
}

ipc_t fifoConnect(ipcdata_t ipcdata){
	ipc_t ret = (ipc_t) malloc(sizeof(struct st_ipc_t));

	if(ret != NULL){
		ret->status = IPCSTAT_CONNECTING;
		ret->ipcdata = ipcdata;
		ret->inbox = qnew();
		ret->outbox = qnew();

		ret->ipcdata->fifodata.fdw = open(ret->ipcdata->fifodata.fifonamew, O_RDWR | O_NONBLOCK);
		ret->ipcdata->fifodata.fdr = open(ret->ipcdata->fifodata.fifonamer, O_RDONLY | O_NONBLOCK);

		if(ret->ipcdata->fifodata.fdw < 0 || ret->ipcdata->fifodata.fdr < 0){
			ret->status = IPCERR_OPENFIFO;
		}else{
			ret->status = IPCSTAT_CONNECTED;
			ret->thread = pthread_create(&(ret->thread), NULL, fifoClientLoop, ret);	
		}
	}
	
	return ret;
}

int writefifo(ipc_t ipc, void * data, int len){
	int nwrite = 0;
	if(ipc->status == IPCSTAT_CONNECTED){
		nwrite = write(ipc->ipcdata->fifodata.fdw, (char *) data, len);
		if(nwrite < 0){
			nwrite = 0;
			//printf("Error escribiendo\n");
		}
	}
	return nwrite;
}

int readfifo(ipc_t ipc, char * buffer, int len){
	int nread = 0;
	if(ipc->status == IPCSTAT_CONNECTED){
		nread = read(ipc->ipcdata->fifodata.fdr, buffer, len);
	}else{
		printf("No se puede leer\n");
	}	
	
	return nread < 0 ? 0 : nread;
}


int fifoDisconnect(ipc_t ipc){
	unlink(ipc->ipcdata->fifodata.fifonamew);
	unlink(ipc->ipcdata->fifodata.fifonamer);
}


void* fifoClientLoop(void* ipcarg){
	ipc_t ipc = (ipc_t) ipcarg;
	
	msg_writting currMsgW = (msg_writting) malloc(sizeof(struct st_msg_writting));
	msg_reading currMsgR = (msg_reading) malloc(sizeof(struct st_msg_reading));

	currMsgW->toWrite = currMsgR->toRead = currMsgR->hdread = 0;
	currMsgR->incomingMsg = (message_t) malloc(sizeof(struct st_message_t));
	
	int offset = 0;
	
	while(ipc->stop != 1){
		if(currMsgW->toWrite == 0){
			message_t nextMsg = qget(ipc->outbox);
			if(nextMsg != NULL){
				currMsgW->toWrite = M_HEADER_SIZE + nextMsg->header.len;
				currMsgW->msglen = currMsgW->toWrite;
				currMsgW->data = mserial(nextMsg);
			}
		}else{
			offset = currMsgW->msglen - currMsgW->toWrite;
			currMsgW->toWrite -= writefifo(ipc, currMsgW->data + offset, currMsgW->toWrite);
		}

		if(currMsgR->toRead == 0){
			currMsgR->hdread += readfifo(ipc, currMsgR->bufferhd + currMsgR->hdread, M_HEADER_SIZE - currMsgR->hdread);
			if(currMsgR->hdread == M_HEADER_SIZE){
				currMsgR->incomingMsg = mhnew( (mheader_t) currMsgR->bufferhd, NULL);
				currMsgR->toRead = currMsgR->incomingMsg->header.len;
				currMsgR->hdread = 0;
			}
		}else{
			offset = currMsgR->incomingMsg->header.len - currMsgR->toRead;
			currMsgR->toRead -= readfifo(ipc, currMsgR->incomingMsg->data + offset, currMsgR->toRead);
			if(currMsgR->toRead == 0){
				qput(ipc->inbox, currMsgR->incomingMsg);
			}  
		}
	}
	
	free(currMsgR->incomingMsg);
	free(currMsgR);
	free(currMsgW);
}

