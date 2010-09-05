#include "../include/ipc_fifo.h"


int main(int argc, char * argv[]){
	if(argc != 2){
		return 1;
	}
	
	int antid = atoi(argv[1]);

	ipc_t client;

	client = fifoConnect(fifoIPCData(antid));
	printf("Cliente iniciado id: %d\n", antid);
	message_t inmsg, aux;
	char word[50];
	sprintf(word, "Mensaje del cliente con id: %d", antid);
	
	while(1){
		printf("Mando mensaje\n");
		qput(client->outbox, (aux = mnew(antid,0,strlen(word) + 1, word)));
		mdel(aux);
		if((inmsg = qget(client->inbox)) != NULL){
			printf("Client recibio: %s\n", inmsg->data);
			mdel(inmsg);
		}
		sleep(1);
	}
	fifoDisconnect(client);
}
