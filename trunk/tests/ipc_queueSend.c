#include "../include/ipc_queue.h"

int main(int argc, char ** argv) {

	int prioOut = 1;
	int prioIn = 2;
	int n;
	message_t msg;
	
	char mtext[200];
	
	ipc_t ipc;
	
	ipc = mq_connect();
	
	while ( (n = read(0, mtext, sizeof mtext)) > 0 )
	{
		msg = mnew(0,0,n,mtext);
		printf("Cliente envia: %.*s", n, mdata(msg));
		//msgsnd(qout, &msg, n, 0);
		mq_sendData(ipc,msg,prioOut);
		//n = msgrcv(qin, &msg, sizeof msg.mtext, msg.mtype, 0);
		msg = mq_getData(ipc, prioIn);
		if ( msg > 0 )
			printf("Cliente recibe: %.*s", mdlen(msg), mdata(msg));
	}
	printf("Cliente termina\n");
	return 0;
	
	/*int pid = 0;
	int priority;
	message_t sendmsg1, sendmsg2;
	ipc_t ipc;
	char * word;
	
	ipc = mq_connect("");
	word = "primero";
	priority = 1;
	sendmsg1 = mnew(0,0,strlen(word),word);
	word = "segundo";
	sendmsg2 = mnew(0,0,strlen(word),word);
	if(mq_sendData(ipc,sendmsg1,priority) < 0){
		printf("warning: enter failure");
		exit(3);
	}
	priority = 2;
	if(mq_sendData(ipc,sendmsg2,priority) < 0){
		printf("warning: enter failure");
		exit(3);
	}
	
	exit(0);*/
	
	/*int priority;
	message_t msg;
	ipc_t ipc;
	
	ipc = mq_connect();
	
	if(argc != 3)
	{
		fprintf(stderr, "usage: %s objname priority\n", argv[0]);
		exit(1);
	}
	
	if((priority = atoi(argv[2])) <= 0 || priority > MAXPRIOR)
	{
		warn("invalid priority");
	}
	
	msg = mnew(0,0,strlen(argv[1]),argv[1]);
	
	if(mq_sendData(ipc,msg,priority) < 0)
	{
		warn("enter failure");
		exit(3);
	}
	
	exit(0);*/
}
