#include "../include/task.h"

int testTasks(char * a)
{
	Task.new(&(System.tasks[Task.findSlot()]), "Task 3", task3, RANK_NORMAL, PRIORITY_LOW, 1);
    Task.new(&(System.tasks[Task.findSlot()]), "Task 1", task1, RANK_NORMAL, PRIORITY_LOW, 1);
    Task.new(&(System.tasks[Task.findSlot()]), "Task 2", task2, RANK_NORMAL, PRIORITY_LOW, 1);
    return 1;
}

int task1 (char* line) {
    int i = 0;
    printf("task 1 started... \n");
    while(i < 100000)
    {
    	i++;
        //Top.run();
    }
	printf("task 1 finishing... \n");
    return 1;
}

int task2 (char* line) {
    int i = 0;
    printf("task 2 started... \n");
    printf("task 2 killing task 3...\n");
    Task.kill(&(System.tasks[5]));
    printf("task 3 killed...\n");
    while(i < 2000)
    {
    	i++;
        //Top.run();
    }
	printf("task 2 finishing... \n");
    return 1;
}

int task3 (char* line) {
    int i = 0;
    printf("task 3 started... \n");
    while(i < 10000000)
    {
    	i++;
        //Top.run();
    }
	printf("task 3 finishing... \n");
    return 1;
}

