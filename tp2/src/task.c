#include "../include/task.h"

int testTasks(char * a)
{
	int slot = Task.findSlot();
		if(slot == -1)
		{
			return -1;
		}
	Task.new(&(System.tasks[slot]), "Task 3", task3, RANK_NORMAL, PRIORITY_LOW, RUNNING_FRONT, System.atty);
		slot = Task.findSlot();
		if(slot == -1)
		{
			return -1;
		}
    Task.new(&(System.tasks[slot]), "Task 1", task1, RANK_NORMAL, PRIORITY_LOW, RUNNING_FRONT, System.atty);
    	slot = Task.findSlot();
		if(slot == -1)
		{
			return -1;
		}
    Task.new(&(System.tasks[slot]), "Task 2", task2, RANK_NORMAL, PRIORITY_LOW, RUNNING_FRONT, System.atty);
    return 1;
}

int task1 (char* line) {
    int i = 0;
    printf("task 1 started... \n");
    while(i < 100000)
    {
    	i++;
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
    }
	printf("task 3 finishing... \n");
    return 1;
}
