#include "../include/ipc_fifo.h"


int main(){
	initFifos(1);
	ipc_t client;

	client = fifoConnect(fifoIPCData(0));
	
	message_t inmsg;
	char * word = "Hola soy el cliente mas nabo que hay mando mensajes al server y recibo los que el me manda";
	
	while(1){
		qput(client->outbox, mnew(0,0,strlen(word) + 1, word));
		if((inmsg = qget(client->inbox)) != NULL){
			printf("Client recibio: %s\n", inmsg->data);
		}
	}
}
