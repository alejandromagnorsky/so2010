#include "../include/ipc_fifo.h"

/*
int main(){
	int pid = 0;
	message_t msg;
	ipc_t ipc;
	
	char * word;
	switch(pid = fork()){
		case -1: return 1;
		case 0:
			ipc = connect("/tmp/fifo1");
			word = "Hola soy un child";
			msg = mnew(0,0,strlen(word), word);
			printf("Envio: %s\n", msg->data);
			sendMessage(ipc, msg);
			printf("Envio: %s\n", msg->data);
			sendMessage(ipc, msg);
			printf("Envio: %s\n", msg->data);
			sendMessage(ipc, msg);
			printf("Envio: %s\n", msg->data);
			sendMessage(ipc, msg);
			
		default:
			ipc = connect("/tmp/fifo1");
			int i = 0;
			while(i != 10){
				ipc = connect("/tmp/fifo1");
				word = "Hola soy el SERVER";
				msg = mnew(0,0,strlen(word), word);
				printf("Envio: %s\n", msg->data);
				sendMessage(ipc, msg);
				msg = getMessage(ipc);
				if(msg != NULL){
					printf("Recibi: %s\n", msg->data);
				}
				i++;
			}
	}
*/

ipc_t createIPCData(int nant){
	ipcdata_t data = malloc(sizeof(un_ipcdata_t));
	if(data != NULL){
		ans->fifonamew = sprintf("/tmp/fifo_c_w_%d", nant);
		ans->fifonamer = sprintf("/tmp/fifo_c_r_%d", nant);
	}
	st_ipc_t ans = malloc(sizeof(st_ipc_t));
	ans->status = IPCSTAT_DISCONNECTED;
	ans->ipcdata = data;
	
	return ans;
}

void connect(ipcdata_t ipc){
	ipc->status = IPCSTAT_CONNECTING;
	ipc->ipcdata->fw = open(ipc->fw, O_RDWR | O_NONBLOCK);
	ipc->ipcdata->fr = open(ipc->fr, O_RDONLY | O_NONBLOCK);
	if(ipc->ipcdata->fw < 0 || ipc->ipcdata->fr < 0){
		ipc->status = IPCSTAT_ERROR;
		return;	
	}
	ipc->status = IPCSTAT_CONNECTED;
}

int writeFifo(ipc_t ipc, void * data, int len){
	int nwrite = 0;
	if(nwrite = write(ipc->ipcdata->fw, (char *) data, len)){
		return 0;
	}
	return nwrite;
}


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

int readfifo(char * fifo, char * buffer, int len){
	return read(fifo, buffer, len);
}

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

int disconnect(ipc_t ipc){
	close(ipc->ipcdata->fifonamew);
	close(ipc->ipcdata->fifonamer);
	unlink(ipc->ipcdata->fr);
	unlink(ipc->ipcdata->fw);
}
