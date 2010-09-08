#include "../include/ipc_shm.h"

void newClient(int antid);
void serverTest(ipc_t server);

int main(){
	
	int semid;
	pid_t pid;
	databuf_t buf1, buf2;
	message_t msgc, msgs;
	int i;
	
	char * msg1 = "Mensaje 1";
	char * msg2 = "Mensaje 2 Mensaje 2 Mensaje 2 Mensaje 2 Mensaje 2 Mensaje 2 Mensaje 2";
	char * msg3 = "Mensaje 3 Mensaje 3";
	char * msg4 = "4";
	char * msg5 = "Mensaje 5 Mensaje 5 Mensaje 5";
	char * msg6 = "m6";
	
	ipc_t server, client;
	
	server = shmServe();
	switch(pid = fork()){
	case 0: 
		for(i = 0; i < 3; i++){
			if(fork() == 0){
				newClient(i);
			}
		}
		break;
	default:
		serverTest(server);
	}
	
}

void newClient(int antid){
	printf("Entra cliente %d\n", antid);
	ipc_t client = shmConnect(shmIPCData(), antid);
	message_t msgc;
	char mensaje[50];
	int i = 0;
	for(i = 0; i < 1; i++){
		sprintf(mensaje, "Este mensaje es de la hormiga %d", antid);
		msgc = mnew(antid,0,strlen(mensaje)+1, mensaje);
		qput(client->outbox, msgc);
		mdel(msgc);
		sleep(1);
	}
	sleep(5);
	client->stop = 1;
	while((msgc = qget(client->inbox)) != NULL){
		printf("Client%d recibe: %s\n",antid,  msgc->data);
		mdel(msgc);
	}
}

void serverTest(ipc_t server){
	int i = 50;
	message_t aux, msgs;
//	while(i != 0){
//		if((aux = qget(server->inbox)) != NULL ){
//			aux->header.to = aux->header.from;
//			qput(server->outbox, aux);
//			mdel(aux);
//		}
//		i++;
//		sleep(1);
//	}
	sleep(5);
	server->stop = 1;
	while((msgs = qget(server->inbox)) != NULL){
		printf("Mensaje en Server: %s\n", msgs->data);
		mdel(msgs);
	}
}