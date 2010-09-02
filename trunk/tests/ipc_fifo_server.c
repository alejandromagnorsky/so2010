#include "../include/ipc_fifo.h"

int main(){
	/*	
	initFifos(1);
	ipc_t server;
	char auxname[50];

	ipcdata_t auxData = fifoIPCData(0);
	strcpy(auxname, auxData->fifodata.fifonamer);
	strcpy(auxData->fifodata.fifonamer, auxData->fifodata.fifonamew);
	strcpy(auxData->fifodata.fifonamew, auxname);	
	
	server = fifoConnect(auxData);
	
	message_t inmsg;
	char * word = "Hola soy un server re pedorro que te mando mensajes y recibo los tuyos";
	
	while(1){
		qput(server->outbox, mnew(0,0,strlen(word) + 1, word));
		if((inmsg = qget(server->inbox)) != NULL){
			printf("server recibio: %s\n", inmsg->data);
		}
		sleep(2);
	}
	*/

	message_t incomingMsg;

	ipc_t server = (ipc_t) fifoServe(NULL, 2);
	server->stop = 0;
	while(1){
		if((incomingMsg = qget(server->inbox)) != NULL){
			printf("Server recibio: %s\n", incomingMsg->data);
		}
	}
}
