#include "../include/ipc_shm.h"

void newClient(int antid);
void serverTest(ipc_t server);

int main(){
	
	int semid;
	pid_t pid;
	databuf_t buf1, buf2;
	message_t msgc, msgs;
	int i;
	
	ipc_t server, client;
	
	server = shmServe();
	if(fork() == 0){
		for(i = 0; i < 3; i++){
			if(fork() == 0){
				newClient(i);
			}
		}
	}else{
		serverTest(server);
	}
	
}

void newClient(int antid){
	ipc_t client = shmConnect(shmIPCData(), antid);
	message_t msgc;
	char mensaje[50];
	int rcc = 0;
	int i = 50;
	while(i-- != 0){
		sprintf(mensaje, "Soy el cliente %d", antid);
		msgc = mnew(antid, 0, strlen(mensaje)+1, mensaje);
		qput(client->outbox, msgc);
		mdel(msgc);
		sleep(1);
	}
	while((msgc = qget(client->inbox)) != NULL){
		printf("%d. Client%d recibio: %s\n", rcc++, antid, msgc->data);
	}
	shmDisconnect(client);
}

void serverTest(ipc_t server){
	int i;
	message_t aux, msgs;
	i = 10000;
	while(i != 0){
		if((aux = qget(server->inbox)) != NULL){
			printf("Server recibio: %s\n", aux->data);
			aux->header.to = aux->header.from;
			qput(server->outbox, aux);
			mdel(aux);
		}
	}
	shmStopServe(server);
}