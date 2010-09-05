#include "../include/ipc_fifo.h"

int main(int argc, char * argv[]){
	
	int cclients = 10;
	int sentMsg = 0;
	printf("Server iniciado para %d clientes.\n", cclients);

	message_t incomingMsg;
	message_t msgsent;
	char * word = "Mensaje del Server";
	
	ipc_t server = (ipc_t) fifoServe(cclients);
	server->stop = 0;

	while(sentMsg != 1000){
		if((incomingMsg = qget(server->inbox)) != NULL){
			printf("%d. Server recibio: %s\n", sentMsg, incomingMsg->data);
			
			printf("Manda mensaje a:%d.\n", incomingMsg->header.from);
			qput(server->outbox, (msgsent = mnew(0,incomingMsg->header.from, strlen(word) + 1, word)));
			mdel(msgsent);

			mdel(incomingMsg);
			sentMsg++;
		}
	}
	stopServer(server);
}
