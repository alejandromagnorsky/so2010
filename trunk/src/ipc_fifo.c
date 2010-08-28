#include "../include/ipc_fifo.h"

int main(){

	ipcdata_t t_ant = fifoIPCData(1);
	ipc_t ipctant = fifoConnect(t_ant);
	printf("Se creó IPC:\n");
	printf("ipc->status: %d\n", ipctant->status);
	printf("ipc->ipcdata->fifodata.fifonamer: %s\n", ipctant->ipcdata->fifodata.fifonamer);
	printf("ipc->ipcdata->fifodata.fifonamew: %s\n", ipctant->ipcdata->fifodata.fifonamew);
	printf("ipc->ipcdata->fifodata.fdr: %d\n", ipctant->ipcdata->fifodata.fdr);
	printf("ipc->ipcdata->fifodata.fdw: %d\n", ipctant->ipcdata->fifodata.fdw);
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
		ret->ipcdata->fifodata.fdw = open(ret->ipcdata->fifodata.fifonamew, O_RDWR | O_NONBLOCK | O_CREAT);	
		ret->ipcdata->fifodata.fdr = open(ret->ipcdata->fifodata.fifonamer, O_RDONLY | O_NONBLOCK | O_CREAT);
		if(ret->ipcdata->fifodata.fdw < 0 || ret->ipcdata->fifodata.fdr < 0){
			ret->status = IPCSTAT_FIFOFILEUSED;
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

int sendMessage(ipc_t ipc, message_t msg){

}

message_t recvMessage(ipc_t ipc){

}


void* fifoClientLoop(void* ipcarg){
	ipc_t ipc = (ipc_t) ipcarg;
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
