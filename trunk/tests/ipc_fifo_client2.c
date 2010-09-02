#include "../include/ipc_fifo.h"


int main(){
	initFifos(1);
	ipc_t client;

	client = fifoConnect(fifoIPCData(1));
	
	message_t inmsg;
	char * word = "Mensaje de Cliente 2";
	
	while(1){
		printf("Client2: Manda mensaje.\n");
		qput(client->outbox, mnew(0,0,strlen(word) + 1, word));
		if((inmsg = qget(client->inbox)) != NULL){
			printf("Client recibio: %s\n", inmsg->data);
		}
		sleep(4);
	}
}
