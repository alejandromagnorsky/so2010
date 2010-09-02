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
			mq_sendData(ipc,msg,2);
		}
	}
	
	/*message_t msg;
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
	}*/
}

void
quit(int sig)
{
	//mq_disconnect(ipc);
	printf("Servidor termina por señal %d\n", sig);
	exit(0);
}
