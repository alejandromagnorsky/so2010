/*
**  SISTEMAS OPERATIVOS, 2C 2010
**  PRIMER TRABAJO PRACTICO
**  Simulacion de una colonia de hormigas.

**  File:       message_test.c
**  Content:    tests for the message library.
*/

#include "../include/message.h"

#include <stdio.h>
#include <stdlib.h>

#define MESSAGE_NUM 500000
#define MESSAGE_DATA "holaHOLAchauCHAUsiSInoNO"

int main(int argc, char** argv) {

    message_t test, copy;
    
    printf("Creating message... ");
    test = newMessage(10, 20 ,sizeof(MESSAGE_DATA), MESSAGE_DATA);
        
    if (!test) {
        printf("message creation failed!\n");
        exit(1);   
    }

    printf ("ok.\n");
    //printMessage(test);
    printf("Verifying created message... ");
    
    if (!(test->from == 10 && test->to == 20
        && test->len == sizeof(MESSAGE_DATA)
        && strncmp(test->data, MESSAGE_DATA, sizeof(MESSAGE_DATA)) == 0)) {
   
        printf("message integrity check failed!.\n");
        exit(1);
    }
    
    printf("ok.\nCreating a copy... ");
    
    copy = copyMessage(test);
    
    if (!copy) {
        printf("message copying failed!\n");
        exit(1);
    }
    
    printf("ok.\n");
    //printMessage(copy);
    printf("Verifying copied message... ");
    
    if (!(copy->from == 10 && copy->to == 20
        && copy->len == sizeof(MESSAGE_DATA)
        && strncmp(copy->data, MESSAGE_DATA, sizeof(MESSAGE_DATA)) == 0)) {
   
        printf("message integrity check failed!.\n");
        exit(1);
    }
    
    printf("ok.\nComparing with original using cmpMessage... ");
    
    if (!cmpMessage(test, copy)) {
        printf("message comparison failed!\n");
        exit(1);
    }
    
    printf("ok.\nFreeing allocated memory using delMessage...");

    delMessage(test);
    delMessage(copy);

    printf("done.\nYou should see a segmentation fault now:\n");
    
    copy->to = 100;
    // porque esto no falla!?!?!?
    printf("If you can read this, message deletion failed!\n");
}
