#ifndef _STRING_H_
#define _STRING_H_

/***************************************************
**
** String manipulation function library header file
**
****************************************************/

char streq(char*, char*); /* Returns 1 if strings are equal, 0 otherwise */
void strcpy(char *s, char* t);
void memzero(void *mem, unsigned int size);
int strlen(char*);
int strcmp(char *, char *);
int atoi(char *buffer, int base);
#endif
