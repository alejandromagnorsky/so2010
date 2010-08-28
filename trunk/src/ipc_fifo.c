#include "../include/ipc_fifo.h"

int main(){

	ipcdata_t t_ant = fifoIPCData(1);
	ipc_t ipctant = fifoConnect(t_ant);
	printf("Se creó el IPC para la hormiga, escribre en: %s, y lee de: %s.\n", ipctant->ipcdata->fifodata.fifonamew, ipctant->ipcdata->fifodata.fifonamew); 
	fifoDisconnect(ipctant);
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
	int rcreat;
	if(ret != NULL){
		ret->status = IPCSTAT_CONNECTING;
		ret->ipcdata = ipcdata;
		ret->inbox = qnew();
		ret->outbox = qnew();

		ret->ipcdata->fifodata.fdw = open(ret->ipcdata->fifodata.fifonamew, O_RDWR | O_NONBLOCK | O_CREAT);	
		ret->ipcdata->fifodata.fdr = open(ret->ipcdata->fifodata.fifonamer, O_RDONLY | O_NONBLOCK | O_CREAT);

		if(ret->ipcdata->fifodata.fdw < 0 || ret->ipcdata->fifodata.fdr < 0){
			ret->status = IPCERR_FIFOFILEUSED;
		}else{
			ret->status = IPCSTAT_CONNECTED;
		}
	}

	rcreat = pthread_create(&(ret->thread), NULL, fifoClientLoop, ret);
	return ret;
}

int writefifo(ipc_t ipc, void * data, int len){
	int nwrite = 0;
	if(ipc->status == IPCSTAT_CONNECTED){
		if((nwrite = write(ipc->ipcdata->fifodata.fdw, (char *) data, len) == -1)){
			return -1;
		}
	}
	return nwrite;
}

int readfifo(ipc_t ipc, char * buffer, int len){
	return read(ipc->ipcdata->fifodata.fdr, buffer, len);
}

int fifoDisconnect(ipc_t ipc){
	unlink(ipc->ipcdata->fifodata.fifonamew);
	unlink(ipc->ipcdata->fifodata.fifonamer);
}

void* fifoClientLoop(void* ipcarg){
	ipc_t ipc = (ipc_t) ipcarg;
	
	char * bufferhd = malloc(M_HEADER_SIZE);	
	
	msg_writting currMsgW = (msg_writting) malloc(sizeof(struct st_msg_writting));
	msg_reading currMsgR = (msg_reading) malloc(sizeof(struct st_msg_reading));

	currMsgW->toWrite = currMsgW->written = currMsgR->read = currMsgR->toRead = 0;

	while(!ipc->stop){
		//sendMessage:
		if((currMsgW->toWrite - currMsgW->written) == 0){
			message_t nextMsg = qget(ipc->outbox);
			if(nextMsg != NULL){
				currMsgW->written = 0;
				currMsgW->toWrite = mfsize(nextMsg);
				currMsgW->data = mserial(nextMsg);
			}
		}else{
			currMsgW->written += writefifo(ipc, (currMsgW->data + currMsgW->written), (currMsgW->toWrite - currMsgW->written)); 
		}

		//receiveMessage:
		if((mfsize(currMsgR->recv) - currMsgR->read) == 0){
			readfifo(ipc, bufferhd, M_HEADER_SIZE);
			currMsgR->recv = mhnew((mheader_t) bufferhd, NULL);
			currMsgR->toRead = currMsgR->recv->header.len;
		}else{
			readfifo(ipc, (currMsgR->recv->data + currMsgR->read), (currMsgR->toRead - currMsgR->read));
		}
	}

	free(bufferhd);
	free(currMsgW);
	free(currMsgR);
}


/*
int writefifo(ipc_t ipc, void * data, int len){
	int nwrite = 0;
	if(ipc->status == IPCSTAT_CONNECTED){
		if((nwrite = write(ipc->ipcdata->fw, (char *) &(data), len)) == -1){
			return -1;
		}
	}else{
		perror("writefifo: Se intenta escribir en un ipc no conectado");
	}	
	return nwrite;	
}
*/

/*
int sendMessage(ipc_t ipc, message_t msg){
	int fd = 0;
	int nwritten = 0;
	int nwrite = 0;	
	int msgSize = msg->len;
	
	fd = open(ipc->path, O_RDWR | O_NONBLOCK);
	
	if(fd < 0){
		return -1;
	}
	
	int msg_len = mfsize(msg);
	if((nwrite = write(fd, (char *) &(msg_len), KEYSIZE)) == -1){
		return -1;
	}else{
		nwritten += nwrite;
	}

	if((nwrite = write(fd, (char *) msg, msg->len)) == -1){
		return -1;
	}else{
		nwritten += nwrite;
	}
	printf("Se han escrito %d bytes\n", nwritten);
	return nwrite;
}
*/

/*
message_t buildMessage(ipc_t){
	int nread = 0;
	
	if(nread = read(ipc->ipcdata->fr,
	
	repetir
		leo HEADER bytes en data (con fifoRead)
	faltan?

	mensaje = mhdeserial(data)
}
*/

/*
message_t getMessage(ipc_t ipc){
	int fd = 0;
	int nt_read = 0;
	int nread = 0;	
	int * key = malloc(sizeof(int));
	
	fd = open(ipc->path, O_RDONLY | O_NONBLOCK);
	
	if(fd < 0){
		return NULL;
	}

	switch(nread = read(fd, (char *) key, KEYSIZE)){
		case -1: 
			printf("Fallo la lectura en el fifo.\n"); 
			return NULL;
		case 0: 
			printf("El fifo está vacio"); 
			return NULL;
		default:
			nt_read += nread;
	}

	int len_message = *((int *) key);

	char * msgBuffer = malloc(len_message);
	

	switch(nread = read(fd, msgBuffer, len_message)){
		case -1: 
			printf("Fallo la lectura en el fifo.\n"); 
			return NULL;
		case 0: 
			printf("El fifo está vacio"); 
			return NULL;
		default:
			nt_read += nread;
	}
	free(key);

	printf("Se han leido %d bytes\n", nt_read);

	return (message_t) msgBuffer;
}
*/
