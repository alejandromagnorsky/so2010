#include "../include/ipc_queue.h"
#include <signal.h>

/* I am 1*/

void quit(int sig);

int main(){
	
	message_t msg;
	ipc_t ipc = mq_connect();
	
	signal(SIGINT, quit);
	
	while (1)
	{
		msg = mq_getData(ipc, 1);
		if (msg > 0 )
		{
			printf("Servidor: %.*s", mdlen(msg), mdata(msg));
			//fflush(stdout);
			//msgsnd(qout, &msg, n, 0);
			msg->header.from=1;
			msg->header.to=2;
			mq_sendData(ipc,msg);
		}
	}
	
	
	/*int pid = 0;
	message_t getmsg;
	ipc_t ipc = mq_connect("");

	switch(pid = fork()){
		case 0:
			getmsg = mq_getData(ipc);
//			printf("getmsg len: %d, getmsg data: %s fin\n", 
											getmsg->len, getmsg->data);
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
		printf("getmsg len: %d, getmsg data: %s fin\n", 
											getmsg->len, getmsg->data);
	}
	
	exit(pid != -1 ? 0 : 1);*/
}

void
quit(int sig)
{
	//mq_disconnect(ipc);
	printf("Servidor termina por señal %d\n", sig);
	exit(0);
}
