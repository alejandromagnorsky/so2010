#include "../include/string.h"

/************************************************
** String manipulation functions:
*************************************************/

char streq(char* s, char* t) {

    for(; *s == *t; s++, t++)
        if (*s == 0)
            return 1;
            
    return 0;

}

void strcpy(char *s, char* t) {
    /* WARNING: JUST AS DANGEROUS AS ITS UNIX COUNTER-PART */
    while(*t++ = *s++);
}
