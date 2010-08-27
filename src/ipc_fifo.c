#include "../include/ipc_fifo.h"

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
}

ipc_t connect(char * path){
	int fd = 0;

	ipc_t newIpc = malloc(sizeof(struct st_ipc_t));
	newIpc->path = malloc(strlen(path));	
	strcpy(newIpc->path, path);
		
	if(mkfifo(path, PERMISSIONS) == -1){
		if(errno != EEXIST){
			return NULL;
		}
	}
	return newIpc;
}

int sendMessage(ipc_t ipc, message_t msg){ //sendData
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

int disconnect(ipc_t ipc){
//	printf("%s cerrado.\n", ipc->path);
	unlink(ipc->path);
}
