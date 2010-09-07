#include "../include/ipc_queue.h"
#include <signal.h>

/*I was 1*/
/* I am SERVER*/

void quit(int sig);

int main(){
	
	ipcdata_t ipcdata;
	ipc_t ipc;
	
	ipcdata = mq_ipcData(SERVERKEY ,SERVERKEY);
	ipc = mq_serve(ipcdata);
	
	message_t msg;
	
	signal(SIGINT, quit);
	
	while(1)
	{
		msg = qget(ipc->inbox);
		if( msg > 0)
		{
			printf("servidor: %.*s", mdlen(msg), mdata(msg));
			//qput(ipc->outbox,msg);
		}
		fflush(stdout);
	}
	
	
	
	
	/* este anda */
	
	/*message_t msg;
	ipc_t ipc = mq_connect(1,1);
	
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
	}*/
	
	/* el de arriba anda */
	
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
