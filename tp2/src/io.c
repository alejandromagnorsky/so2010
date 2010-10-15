#include "../include/io.h"

/************************************************
** I/O primitives (should NOT be used directly):
*************************************************/

void _memcpy(void* from, void* to, size_t nbytes) {
/* Copies bytes from a buffer to another */
    size_t i;
    for (i = 0; i < nbytes; i++)
        * ((char *)to + i) = * ((char *)from + i);
}

unsigned char _inport(unsigned char port) {
/* Reads a byte from an I/O port */
    char c;

    __asm__("in %%dx, %%al;"            /* Assembler instructions */
            :"=a" (c)                   /* Output variables */
            :"d" (port)                 /* Input variabless */
           );
    
    return c;
}

void _outport(unsigned char port, unsigned char c) {
/* Writes a byte to an I/O port */

    __asm__("out %%al, %%dx;"           /* Assembler instructions */
            :                           /* Output variables */
            :"d" (port), "a" (c)        /* Input variabless */
           );
}



/************************************************
** Driver abstraction layer:
*************************************************/

struct driver_t KeyboardDriver = {
    _kb_write,
    _kb_read, /* the rest is null as well */
};

size_t _kb_read(int devcode, void* to, size_t nbytes) {
    return _dread(System.device[devcode], to, nbytes);
}

size_t _kb_write(int devcode, void* from, size_t nbytes) {
    return _dwrite(System.device[devcode], from, nbytes);
}

struct driver_t ScreenDriver = {
    _screen_write
};

size_t _screen_write(int devcode, void* from, size_t nbytes) {
    return _dwrite(System.device[devcode], from, nbytes);
}

struct driver_t TtyDriver = {
	_screen_write
};

/************************************************
** Device interface:
*************************************************/

size_t _dread(device_t dev, void* to, size_t nbytes) {

    switch (dev->id) {
    
        case DEVICE_KEYBOARD:
            // To simplify matters, keyboard buffer is read 1 byte at a time.
            
            nbytes = (dev->rpos != dev->wpos) ? 1 : 0;
            
            if (nbytes > 0) { /* Key data available */
                if (dev->rpos + 1 > dev->size)
                    dev->rpos = 0; // This makes the buffer circular
                
                _memcpy(dev->addr + dev->rpos, to, 1);
                dev->rpos++;
            }
            break;
    
        case DEVICE_SCREEN:
            return 0;
            break;
            
        default:
            if (dev->rpos + nbytes > dev->size)
                nbytes = (dev->size - dev->rpos);
            
            _memcpy(dev->addr + dev->rpos, to, nbytes);
            dev->rpos += nbytes;
            break;
    }
    
    return nbytes;
}

size_t _dwrite(device_t dev, void* from, size_t nbytes) {

    size_t left;
    size_t wnow;

	size_t wpos;
	char zero = 0;
	int colpos;
	unsigned char format = 0x07;
	int i;
    switch (dev->id) {
                   
        case DEVICE_KEYBOARD:
            // To simplify matters, keyboard buffer is written 1 byte at a time.
            nbytes = 1;
            
            if (dev->wpos + 1 > dev->size)
               dev->wpos = 0; // This makes the buffer circular.
            
            _memcpy(from, dev->addr + dev->wpos, 1);
            dev->wpos++;
            break;

		case DEVICE_TTY:
        case DEVICE_SCREEN:
			return video_write(dev,from,nbytes);        
        default:
            if (dev->wpos + nbytes > dev->size)
                nbytes = (dev->size - dev->wpos);
                
            _memcpy(from, dev->addr + dev->wpos, nbytes);
            dev->wpos += nbytes;
            break;
    }
    return nbytes;
}

size_t _dtellw(device_t dev) {
    return dev->wpos;
}

size_t _dtellr(device_t dev) {
    return dev->rpos;
}

size_t _dseekw(device_t dev, int offset, int from) {
    size_t newpos;
    
    switch(from) {
        case DEVICE_START:
            if (offset >= 0) {
                if ((newpos = offset) >= dev->size)
                    newpos = dev->size - 1;
            } else
                newpos = 0;
                
            break;
       
        case DEVICE_CURRENT:
            if (offset < 0) {
                if ((newpos = dev->wpos + offset) < 0)
                    newpos = 0;
            
            } else {
                if ((newpos = dev->wpos + offset) >= dev->size)
                    newpos = dev->size - 1;
            }
            break;
            
        case DEVICE_END:
            if (offset <= 0) {
                if ((newpos = dev->size - 1 + offset) < 0)
                    newpos = 0;
                    
            } else
                newpos = dev->size - 1;
    }
    
    return (dev->wpos = newpos);
}

size_t _dseekr(device_t dev, int offset, int from) {
    size_t newpos;

    switch(from) {
        case DEVICE_START:
            if (offset >= 0) {
                if ((newpos = offset) >= dev->size)
                    newpos = dev->size - 1;
            } else
                newpos = 0;
                
            break;
       
        case DEVICE_CURRENT:
            if (offset < 0) {
                if ((newpos = dev->rpos + offset) < 0)
                    newpos = 0;
            
            } else {
                if ((newpos = dev->rpos + offset) >= dev->size)
                    newpos = dev->size - 1;
            }
            break;
            
        case DEVICE_END:
            if (offset <= 0) {
                if ((newpos = dev->size - 1 + offset) < 0)
                    newpos = 0;
                    
            } else
                newpos = dev->size - 1;
    }
    
    return (dev->rpos = newpos);
}

/************************************************
** Higher level IO functions:
*************************************************/

void putchar(char c) {
    static char ch[2] = {0, 0x07};
    ch[0] = c;
	System.write(DEVICE_TTY, ch, 1);
	System.write(DEVICE_SCREEN, ch, 1);			
}

void puts(char* str) {
    while (*str != 0)
        putchar(*str++);
}

void putd(int d) {
    if (d < 0) {
        putchar('-');
        d = 0 - d;
    }
    putu(d);
}

void putu(unsigned int u) {
    if (u >= 10) putu(u / 10);
    putchar(u % 10 + '0');
}

void printf(char* fmt, ...) {

    void *param;    
    char *c;
    
    param = &fmt + 1;

    for (c = fmt; *c != 0; c++) {
       if (*c == '%') {
                
            switch( *(++c) ) {
                case 0:
                    /* End of string! bad format. Just ignore the symbol... */
                    c--; /* ... and step back so the outer loop quits */
                    break;
                    
                case '%':
                    putchar('%');
                    break;

                case 'd':
                    putd(* (int*) param);
                    param += sizeof(int*);
                    break;

                case 'u':
                    putu(* (unsigned int*) param);
                    param += sizeof(unsigned int*);
                    break;
                    
                case 's':
                    puts(* (char**) param);
                    param += sizeof(char**);
                    break;
            }
            
        } else
            putchar(*c);
    }
 
}

char isNumeric(char c) {
    return (c >= '0' && c <= '9');
}

int sscanf(char* str, char* fmt, ...) {
    int ret;
    void* param;
    char *cs, *cf, *cparam; /* Pointers to read str and fmt */
    
    ret = 0;
    param = &fmt + 1;

    for (cs = str, cf = fmt; *cf != 0 && *cs != 0; cf++) {
        if (*cf == '%') {
            switch ( *(++cf) ) {

                case 0:
                    /* Bad format! Just ignore the symbol and step back,
                       so the loop ends */
                    cf--;
                    break;
                    
                case '%':
                    if (*cs == '%')
                        cs++;
                    else
                        return ret;
                    break;
                
                case 'd':
                    if (isNumeric(*cs)) {
                        ** (int**) param = 0;
                        do {
                            ** (int**) param *= 10;
                            ** (int**) param += (*cs - '0');               
                        // This can overflow...
                        } while (isNumeric(*(++cs)));
                        
                        param += sizeof(int*);
                        ret++;
                        
                    } else
                        return ret;
                        
                case 's':
                    *cf++; /* cf now holds the end delimitator for the string */
                    cparam = * (char**) param; /* cparam points to string buf */
                    
                    while( (*cs != *cf) && (*cs != 0) )
                        * ((char*) cparam++) = *(cs++);
                        
                    * (char*) cparam++ = 0;
                    
                    if (*cs != 0) *cs++; /* If not at the end, skip delim. chr */
                    
                    param+= sizeof(char*);
                    ret++;
            }
        
        } else {
            if (*cs == *cf)
                cs++;
            else
                return ret;
        }
        
    }
    
    return ret;
    
}

char printable(char c) {
    return ( (c > 31 && c < 127) || c == '\b' || c == '\n' );
}

unsigned char getchar() {
    unsigned char c;
    /* Block until an ascii character is read: */   
    while ( (System.read(DEVICE_KEYBOARD, &c, 1) == 0) );
    return c;
}

int scanf(char* fmt, ...) {
    int ret, pos;
    void* param;
    static char str[LINEBUF_LEN];
    char c, *cs, *cf, *cparam;
    
    ret = pos = 0;
    param = &fmt + 1;

    /* First, we get a string from the user: */
    
    while( (pos < LINEBUF_LEN - 1) && ((c = getchar()) != '\n') ) {
        str[pos++] = c;
        putchar(c);
    }
    
    str[pos] = 0;
    
    /* Here sscanf should be called, but how to pass a variable number of
       arguments down to it beats me. So it's functionality was duplicated. */
    
    for (cs = str, cf = fmt; *cf != 0 && *cs != 0; cf++) {
        if (*cf == '%') {
            switch ( *(++cf) ) {

                case 0:
                    /* Bad format! Just ignore the symbol and step back,
                       so the loop ends */
                    cf--;
                    break;
                    
                case '%':
                    if (*cs == '%')
                        cs++;
                    else
                        return ret;
                    break;
                
                case 'd':
                    if (isNumeric(*cs)) {
                        ** (int**) param = 0;
                        do {
                            ** (int**) param *= 10;
                            ** (int**) param += (*cs - '0');               
                        // This can overflow...
                        } while (isNumeric(*(++cs)));
                        
                        param += sizeof(int*);
                        ret++;
                        
                    } else
                        return ret;
                        
                case 's':
                    *cf++; /* cf now holds the end delimitator for the string */
                    cparam = * (char**) param; /* cparam points to string buf */
                    
                    while( (*cs != *cf) && (*cs != 0) )
                        * ((char*) cparam++) = *(cs++);
                        
                    * (char*) cparam++ = 0;
                    
                    if (*cs != 0) *cs++; /* If not at the end, skip delim. chr */
                    
                    param+= sizeof(char*);
                    ret++;
            }
        
        } else {
            if (*cs == *cf)
                cs++;
            else
                return ret;
        }
        
    }
    
    return ret;
    
}
