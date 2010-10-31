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

void strncpy(char* s, char* t, int n) {

    int i = 0;
    while ((*t++ = *s++) && (++i < n));

}

void memzero(void *mem, unsigned int size) {
    char *pos, *end;
    pos = (char*) mem;
    end = (char*) mem + size;
    
    while (!(*pos = 0) && ++pos < end);
}

int
strcmp(char * s1, char * s2)
{
	int diff = 0 ;

	while(*s1 != '\0' && *s2 != '\0' && diff == 0)
		diff += *s1++ - *s2++ ;    

	if ( diff == 0 && *s1 - *s2 < 0 )
		return -1 ;
	if ( diff == 0 && *s1 - *s2 > 0 )
		return 1 ;

	return diff ;
}

int
strlen(char* str)
{
	int length = 0 ;

	while ( str[length] != '\0' )
		length++ ;

	return length ;
}

int
atoi(char *buffer, int base)
{
   int numero = 0;
   int s = 0;
      
   if(buffer == 0)
   {
      return -1;
   }
      
   while(*buffer != '\0')
   {         
      if(*buffer >= '0' && *buffer <= '9')
      {
         s = *buffer - '0';   
         numero = (numero* base) + s;
      }
      buffer++;
   }
   return numero;
}
