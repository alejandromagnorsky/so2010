#include "../include/task.h"

int testTasks(char * a) {
   //System.name("Testing");
    System.exec(task1, "mensaje de prueba");
    
}

int task1(char* line) {

	//System.name("Test Task1");

    printf("%s\n", line);
    System.sleep(50);
    printf("%s\n", line);
	System.sleep(100);
    printf("%s\n", line);
    System.sleep(150);
    printf("%s\n", line);
    System.sleep(200);
    printf("%s\n", line);
    
	printf("Trying to kill idle: ");
    System.kill(1);
    
    /*printf("Test Task1 is going to kill itself\n");
    System.kill(7);
    
    printf("This shouldnt be printed, i should be dead\n");*/
}
