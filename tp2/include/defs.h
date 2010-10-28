/***************************************************
  Defs.h
	
****************************************************/

#ifndef _defs_
#define _defs_

#define NULL 0

#define TRUE    1
#define FALSE   0

#define bool    unsigned char
#define uint8   unsigned char
#define uint16  unsigned short int
#define uint32  unsigned int

#define size_t int
#define ssize_t unsigned short

#define forever for(;;)
#define skip    if(0)

#define byte unsigned char
#define word short int
#define dword int

/* Flags para derechos de acceso de los segmentos */
#define ACS_PRESENT     0x80            /* segmento presente en memoria */
#define ACS_CSEG        0x18            /* segmento de codigo */
#define ACS_DSEG        0x10            /* segmento de datos */
#define ACS_READ        0x02            /* segmento de lectura */
#define ACS_WRITE       0x02            /* segmento de escritura */
#define ACS_IDT         ACS_DSEG
#define ACS_INT_386 	0x0E		/* Interrupt GATE 32 bits */
#define ACS_INT         ( ACS_PRESENT | ACS_INT_386 )


#define ACS_CODE        (ACS_PRESENT | ACS_CSEG | ACS_READ)
#define ACS_DATA        (ACS_PRESENT | ACS_DSEG | ACS_WRITE)
#define ACS_STACK       (ACS_PRESENT | ACS_DSEG | ACS_WRITE)

#pragma pack (1) 		/* Alinear las siguiente estructuras a 1 byte */

/* Descriptor de segmento */
typedef struct {
  word limit,
       base_l;
  byte base_m,
       access,
       attribs,
       base_h;
} DESCR_SEG;


/* Descriptor de interrupcion */
typedef struct {
  word      offset_l,
            selector;
  byte      cero,
            access;
  word	    offset_h;
} DESCR_INT;

/* IDTR  */
typedef struct {
  word  limit;
  dword base;
} IDTR;

/* Registros (Pushed by the ASM handler on Exceptions) */
struct regs
{
    unsigned int gs, fs, es, ds;      /* pushed the segs last */
    unsigned int edi, esi, ebp, esp, ebx, edx, ecx, eax;  /* pushed by 'pusha' */
    unsigned int int_no, err_code;    /* our 'push byte #' and ecodes do this */
    unsigned int eip, cs, eflags, useresp, ss;   /* pushed by the processor automatically */ 
};



/* <ttys> */

#define NTTYS 4

#define ROWS 25
#define COLS 80
#define VIDEO_SIZE (ROWS * COLS * 2)
#define LAST_ROW VIDEO_SIZE - 2 * COLS

#define LINEBUF_LEN 100

#define BUFFER_SIZE 1000

enum{
	NO_TTY = -1,
	TTY0 = 0,
	TTY1,
	TTY2,
	TTY3
};

typedef struct {
    char name[LINEBUF_LEN];
    char args[LINEBUF_LEN - 2];
} command_t;

typedef struct {
    char line[LINEBUF_LEN];
    int pos;
} linebuffer_t;


typedef struct{
	unsigned char shift_status;
	unsigned char mayus_status;
	unsigned char num_status;
	unsigned char scroll_status;
	unsigned char accent_status;
	unsigned char escaped;
}flags_t;

typedef struct {
	char address[VIDEO_SIZE];
	size_t wpos;
	size_t rpos;
}screen_t;

typedef struct{
	unsigned char address[BUFFER_SIZE];
	size_t wpos;
	size_t rpos;
}inputbuffer_t;

typedef struct {
	flags_t flags;
	inputbuffer_t inputbuffer;
}input_t;

typedef screen_t output_t;

typedef struct {
	int status;
	input_t input;
	output_t output;
}tty_t;

enum {
	TTY_READY = 0,
	TTY_WRITTEN
};

/* </ttys> */

#endif
