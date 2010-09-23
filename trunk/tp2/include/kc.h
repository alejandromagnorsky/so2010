/**********************
 kc.h
**********************/
#include "defs.h"

#ifndef _kc_
#define _kc_

#define WHITE_TXT 0x07 // Atributo de video. Letras blancas, fondo negro

/* Muestra la imagen de inicio */
void showSplashScreen();

/* Tiempo de espera */
void wait(int time);

/* Limpia la pantalla */
void k_clear_screen();

/* Inicializa la entrada del IDT */
void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero);

/* Datos PIC */     
#define PIC1		0x20 	/* Direccion de IO del PIC1 */
#define PIC2		0xA0	/* Direccion de IO del PIC2 */
#define PIC1_COMMAND	PIC1
#define	PIC2_COMMAND	PIC2
#define PIC1_DATA	(PIC1+1)
#define	PIC2_DATA	(PIC2+1)

#define	ICW1		0x11
#define	ICW4_8086	0x01

#endif
