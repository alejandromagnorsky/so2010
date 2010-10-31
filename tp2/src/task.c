#include "../include/task.h"

int testTasks(char * a) {
    
    System.exec(task1, "mensaje de prueba");
    
}

int task1(char* line) {

    printf("%s\n", line);
    System.sleep(50);
    printf("%s\n", line);
System.sleep(100);
    printf("%s\n", line);
    System.sleep(150);
    printf("%s\n", line);
    System.sleep(200);
    printf("%s\n", line);
}
