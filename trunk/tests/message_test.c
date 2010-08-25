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
#define MESSAGE_DATA_SIZE (sizeof(MESSAGE_DATA) - 1)

int main(int argc, char** argv) {

    char* serialized;
    message_t test, copy, deserialized;
    
    printf("Creating message... ");
    test = mnew(MESSAGE_DATA_SIZE, MESSAGE_DATA);
        
    if (!test) {
        printf("message creation failed!\n");
        exit(1);   
    }

    printf ("ok.\n");
    //printMessage(test);
    printf("Verifying created message... ");
    
    if (!(test->len == MESSAGE_DATA_SIZE
        && strncmp(test->data, MESSAGE_DATA, MESSAGE_DATA_SIZE) == 0)) {
   
        printf("message integrity check failed!.\n");
        exit(1);
    }
    
    printf("ok.\nCreating a copy... ");
    
    copy = mcopy(test);
    
    if (!copy) {
        printf("message copying failed!\n");
        exit(1);
    }
    
    printf("ok.\n");

    printf("Verifying copied message... ");
    
    if (!(copy->len == MESSAGE_DATA_SIZE
        && strncmp(copy->data, MESSAGE_DATA, MESSAGE_DATA_SIZE) == 0)) {
   
        printf("message integrity check failed!.\n");
        exit(1);
    }
    
    printf("ok.\nComparing with original using mcmp... ");
    
    if (!mcmp(test, copy)) {
        printf("message comparison failed!\n");
        exit(1);
    }
    
    printf("ok.\nSerializing and de-serializing into a new copy... ");

    serialized = mserial(test);
    deserialized = mdeserial(serialized);
    
    printf("ok.\nComparing with original using mcmp... ");

    if (!mcmp(test, deserialized)) {
        printf("message comparison failed!\n");
        exit(1);
    }

    printf("ok.\nFreeing allocated memory using mdel... ");

    free(serialized);
    mdel(test);
    mdel(copy);
    mdel(deserialized);
        
    printf("apparently ok.\nTest went ok!\n\n");
}
