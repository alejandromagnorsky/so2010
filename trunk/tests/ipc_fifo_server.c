#include "../include/ipc_fifo.h"

int main(int argc, char * argv[]){

	int cclients = atoi(argv[1]);
	initFifos(cclients);

	printf("Server iniciado para %d clientes.\n", cclients);

	message_t incomingMsg;

	char word[50];
	int i;
	
	ipc_t server = (ipc_t) fifoServe(NULL, cclients);
	server->stop = 0;

	sprintf(word, "Bienvenida al server de hormigas, usted es ant_id: %d", 0);

	while(1){
		if((incomingMsg = qget(server->inbox)) != NULL){
			printf("Server recibio: %s\n", incomingMsg->data);
			qput(server->outbox, mnew(incomingMsg->header.from,-1,strlen(word) + 1, word));
		}
		
	}
}
