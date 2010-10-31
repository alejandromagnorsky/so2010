#include "../include/task.h"

int testTasks(char * a) {

    System.name("testTasks");

    System.exec(task2, "hola");
    System.exec(task3, "chau chau chau");
    
}

int task1(char* line) {
    // Esto prueba el sleep
    System.name("testTask 1");
	System.name("Test Task1");

    printf("%s\n", line);
    System.sleep(50);
    printf("%s\n", line);
    System.sleep(100);
    printf("%s\n", line);
    System.sleep(150);
    printf("%s\n", line);
    System.sleep(200);
    printf("%s\n", line);
    
	/*printf("Trying to kill idle: ");
    System.kill(1);
    
    printf("Test Task1 is going to kill itself\n");
    System.kill(7);
    
	System.sleep(200);
    printf("This shouldnt be printed, i should be dead\n"); */
}

int task2(char* line) {
    
    System.name("testTask 2");
    System.send(System.gettid("testTask 3"), "prueba", 7);
    while(1);
}

int task3(char* line) {
    int len;
    char msg[128];
    
    System.name("testTask 3");
    len = System.recv();
    System.getmsg(msg, len);

    printf("Len: %d, msg: %s\n", msg, len);

    System.clsmsg();

}
