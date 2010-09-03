#include "../include/ipc_fifo.h"


int main(int argc, char * argv[]){
	if(argc != 2){
		return 1;
	}
	
	int antid = atoi(argv[1]);

	ipc_t client;

	client = fifoConnect(fifoIPCData(antid));
	printf("Cliente iniciado id: %d\n", antid);
	message_t inmsg;
	char word[50];
	sprintf(word, "Mensaje del cliente con id: %d", antid);
	
	while(1){
		printf("mando mensaje\n");
		qput(client->outbox, mnew(antid,-1,strlen(word) + 1, word));
		if((inmsg = qget(client->inbox)) != NULL){
			printf("Client recibio: %s\n", inmsg->data);
		}
		sleep(3);
	}
}
