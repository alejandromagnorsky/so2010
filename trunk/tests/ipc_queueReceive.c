#include "../include/ipc_queue.h"


int main(){
	
	message_t msg;
	ipc_t ipc = mq_connect("");
	
	int prioIn = 1;
	int prioOut = 2;

	while (1)
	{
		msg = mq_getData(ipc, prioIn);
		if (msg > 0 )
		{
			printf("Servidor: %.*s", mdlen(msg), mdata(msg));
			//msgsnd(qout, &msg, n, 0);
			mq_sendData(ipc,msg,prioOut);
		}
	}

	/*int pid = 0;
	message_t getmsg;
	ipc_t ipc = mq_connect("");

	switch(pid = fork()){
		case 0:
			getmsg = mq_getData(ipc);
//			printf("getmsg len: %d, getmsg data: %s fin\n", getmsg->len, getmsg->data);
			break;
		case -1:
			printf("warning: fork to start server failed\n");
			break;
		default:
			printf("server process pid is %d\n",pid);
	}*/
	
	/*int pid = 0;
	message_t getmsg;
	ipc_t ipc = mq_connect("");
	
	while(1)
	{
		getmsg = mq_getData(ipc);
		printf("getmsg len: %d, getmsg data: %s fin\n", getmsg->len, getmsg->data);
	}
	
	exit(pid != -1 ? 0 : 1);*/
}
