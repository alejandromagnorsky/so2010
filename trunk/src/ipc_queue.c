#include "../include/ipc_queue.h"

#ifndef _ipc_queue_
#define _ipc_queue_

int main(){

	int pid = 0;
	message_t getmsg;
	ipc_t ipc;
	
	ipc = connect("");

	switch(pid = fork()){
		case 0:
			getmsg = getMessage(ipc);
			
			printf("getmsg len: %d, getmsg data: %s fin\n", getmsg->len, getmsg->data);
			break;
		case -1:
			printf("warning: fork to start server failed\n");
			break;
		default:
			printf("server process pid is %d\n",pid);
	}
	exit(pid != -1 ? 0 : 1);
}

/*int main() {
	int pid = 0;
	message_t sendmsg1, sendmsg2;
	ipc_t ipc;
	char * word;
	
	ipc = connect("");
	word = "primero";
	sendmsg1 = newMessage(0,0,strlen(word),word);
	word = "segundo";
	sendmsg2 = newMessage(0,0,strlen(word),word);
	if(sendMessage(ipc,sendmsg1) < 0){
		printf("warning: enter failure");
		exit(3);
	}
	if(sendMessage(ipc,sendmsg2) < 0){
		printf("warning: enter failure");
		exit(3);
	}
	
	exit(0);	
}*/

ipc_t connect(char * path){
	int queue_id;
	ipc_t newIpc = malloc(sizeof(struct st_ipc_t));
	
	if((queue_id = msgget(QKEY, IPC_CREAT | QPERM)) == -1){
		perror("msgget failed");
		newIpc->status = -1;
	}
	newIpc->status = 0;
	newIpc->id = queue_id;
	return newIpc;
}

int sendMessage(ipc_t ipc, message_t msg){

	if(msgsnd(ipc->id, &msg->data, msg->len,0) == -1){
		perror("msgsnd failed");
		return -1;
	} 
	else {
		return 0;
	}
}

message_t getMessage(ipc_t ipc){
	int mlen;
	char * mdata;
	message_t msg;
	/*while(1){*/
	printf("antes\n");
	printf("%d",ipc->id);
	mlen = msgrcv(ipc->id, &mdata, MAXOBN,0, MSG_NOERROR);
	printf("asdfasfa\n");
		if(mlen == -1){

			perror("msgrcv failed");
			return newMessage(0,0,0,"");
		}
		else {
			mdata[mlen] = '\0';
			msg = newMessage(0,0,mlen,mdata);
			
			return msg;
		}
	//}
	
}

int disconnect(ipc_t ipc){
	/* existe? */
}

#endif
