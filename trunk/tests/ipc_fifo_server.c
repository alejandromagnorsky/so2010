#include "../include/ipc_fifo.h"

/*
int main(){
	
	initFifos(1);
	char auxname[50];
	message_t inmsg;


	ipcdata_t auxData = fifoIPCData(0);

	strcpy(auxname, auxData->fifodata.fifonamer);
	strcpy(auxData->fifodata.fifonamer, auxData->fifodata.fifonamew);
	strcpy(auxData->fifodata.fifonamew, auxname);

	ipc_t server = fifoConnect(auxData); 
	
	strcpy(server->ipcdata->fifodata.fifonamer, "/tmp/fifo_c_w_0");
	strcpy(server->ipcdata->fifodata.fifonamew, "/tmp/fifo_c_r_0");

	char * msg1 = calloc(sizeof(char), 10);
	strcpy(msg1, "s.msg");

	char * word = "Soy el server y le mando este mensaje al server porque me la banco";

	char a;

	fifoConnect(server->ipcdata);

	while(1){
		qput(server->outbox, mnew(0,0,strlen(word)+1, word));
		inmsg = qget(server->inbox);
		if(inmsg != NULL){
			printf("Server recibio: %s\n", inmsg->data);
		}
		sleep(2);	
	}	
}
*/

int main(){
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
	}
}
