#include "../include/ipc_fifo.h"


int main(){
	initFifos(1);
	ipc_t client;

	client = fifoConnect(fifoIPCData(0));
	
	message_t inmsg;
	char * word = "Mensaje Cliente 1";
	
	while(1){
		printf("mando mensaje\n");
		qput(client->outbox, mnew(0,0,strlen(word) + 1, word));
		if((inmsg = qget(client->inbox)) != NULL){
			printf("Client recibio: %s\n", inmsg->data);
		}
		sleep(3);
	}
}
