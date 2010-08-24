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
			msg = newMessage(0,0,strlen(word), word);
			printf("Envio: %s\n", msg->data);
			sendMessage(ipc, msg);
			sleep(2);
			printf("Envio: %s\n", msg->data);
			sendMessage(ipc, msg);
			break;
		default:
			ipc = connect("/tmp/fifo1");
			int i = 0;
			while(i != 100){
				msg = getMessage(ipc);
				if(msg != NULL){
					printf("Recibi: %s\n", msg->data);
				}
				sleep(1);
				i++;
			}
	}

}
*/

ipcdata_t createIPCData(int nant){
	ipcdata_t ans = malloc(sizeof(un_ipcdata_t));
	if(ans != NULL){
		ans->fw = sprintf("/tmp/fifo_c_w_%d", nant);
		ans->fr = sprintf("/tmp/fifo_c_r_%d", nant);
	}
	return ans;
}

/*
ipc_t connect(char * path){
	int fd = 0;

	ipc_t newIpc = malloc(sizeof(struct st_ipc_t));
	newIpc->path = malloc(strlen(path));	
	strcpy(newIpc->path, path);
	newIpc->status = 0;
		
	if(mkfifo(path, PERMISSIONS) == -1){
		if(errno != EEXIST){
			newIpc->status = -1;
			return newIpc;
		}
	}
	
	return newIpc;
}
*/

void connect(ipcdata_t ipc){
	ipc->fw = open(ipc->fw, O_RDWR | O_NONBLOCK);
	ipc->fr = open(ipc->fr, O_RDONLY | O_NONBLOCK);
}
/*
int sendMessage(ipc_t ipc, message_t msg){
	int fd = 0;
	int nwritten = 0;
	int nwrite = 0;	
	char keyBuf[KEYSIZE] = {0,0,0,0};
	int msgSize = intToKey(keyBuf, msg->len);
	
//	printf("Abriendo %s para escritura.\n", ipc->path);
	
	fd = open(ipc->path, O_RDWR | O_NONBLOCK);
	
	if(fd < 0){
//		printf("Falla para abrir %s, para escribir.\n", ipc->path);
		return -1;
	}
	
	if((nwrite = write(fd, keyBuf, KEYSIZE)) == -1){
//		printf("Falla la escritura en el fifo.\n");
		return -1;
	}else{
		nwritten += nwrite;
	}

	char * msgBuffer = malloc(msg->len);
	strcpy(msgBuffer, msg->data);

	if((nwrite = write(fd, msgBuffer, msg->len)) == -1){
//		printf("Falla la escritura en el fifo.\n");
		return -1;
	}else{
		nwritten += nwrite;
	}

//	printf("Se han escrito %d bytes en el %s\n", nwritten, ipc->path);

	return nwrite;
}
*/

int sendMessage(ipcdata_t ipc, message_t msg){

}

int getMessage(ipcdata_t ipc, message_t msg){

}

/*
message_t getMessage(ipc_t ipc){
	int fd = 0;
	int nt_read = 0;
	int nread = 0;	
	char keyBuf[KEYSIZE] = {0,0,0,0};
	
	fd = open(ipc->path, O_RDONLY | O_NONBLOCK);
	
	if(fd < 0){
//		printf("Falla para abrir %s, para leer.\n", ipc->path);
		return NULL;
	}

	switch(nread = read(fd, keyBuf, KEYSIZE)){
		case -1: 
//			printf("Fallo la lectura en el fifo.\n"); 
			return NULL;
		case 0: 
//			printf("El fifo está vacio"); 
			return NULL;
		default:
			nt_read += nread;
	}

	int len_message = keyToInt(keyBuf);
	
//	printf("Se leyo la key: %d\n", len_message);

	char * msgBuffer = malloc(len_message);
	

	switch(nread = read(fd, msgBuffer, len_message)){
		case -1: 
//			printf("Fallo la lectura en el fifo.\n"); 
			return NULL;
		case 0: 
//			printf("El fifo está vacio"); 
			return NULL;
		default:
			nt_read += nread;
	}

	close(fd);

//	printf("Se han leido %d bytes de %s\n", nt_read, ipc->path);

	return newMessage(0, 0, len_message, msgBuffer);
}
*/

int disconnect(ipc_t ipc){
	unlink(ipc->path);
}
