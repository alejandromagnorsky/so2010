#include "../include/task.h"

static char* x = "Task 2 (Msg len:%d) \"%s\"\n";
static char* y = "Task 3 (Msg len:%d) \"%s\"\n";

static char* test_message = "prueba";
static char* test_message2 = "uno mas largo";
static char* test_message3 = "aca tenes largo, gil";
static char* basta = "basta";

int testTasks(char * a) {

    System.name("testTasks");

    System.exec(task3, "3");
    System.sleep(100);
    System.exec(task2, "2");
    
}

int task1(char* line) {
    // Esto prueba el sleep
    System.name("1");
    
}

int task2(char* line) {
    int a, len;
    char msg[128];
    System.name("2");

    while ((a = System.gettid("3")) == 0);
    
    System.send(a, test_message, 7);
    System.send(a, test_message2, 14);
    System.send(a, basta, 6);
    
    len = System.recv();

    System.getmsg(msg, len);

    printf(x, len, msg);

    System.clsmsg();
    
}

int task3(char* line) {
    int i, len, stop = 0;
    char msg[128];
    
    System.name("3");

    while(!stop) {
        len = System.recv();
        System.getmsg(msg, len);

        printf(y, len, msg);

        System.clsmsg();

        stop = streq(msg, basta);
        
    }

    System.send(System.gettid("2"), test_message3, 21);
    System.sleep(100);
    System.sleep(1);
    printf("a");
}



int doNothing4Times(char* line) {
	System.name("doNothing1");
	System.exec(doNothing2, "2");
    for(;;);
    return 0;
}

int doNothing2(char * line)
{
	System.name("doNothing2");
	System.exec(doNothing3, "3");
	for(;;);
	return 0;
}

int doNothing3(char * line)
{
	System.name("doNothing3");
	System.exec(doNothing4, "4");
	for(;;);
	return 0;
}

int doNothing4(char * line)
{
	System.name("doNothing4");
	for(;;);
	return 0;
}
