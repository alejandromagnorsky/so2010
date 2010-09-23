#include "../include/kc.h"


/***************************************************************
*k_clear_screen
*
* Borra la pantalla en modo texto color.
****************************************************************/

void k_clear_screen() 
{
	char *vidmem = (char *) 0xb8000;
	unsigned int i=0;
	while(i < (80*25*2))
	{
		vidmem[i]=' ';
		i++;
		vidmem[i]=WHITE_TXT;
		i++;
	};
}

/***************************************************************
*setup_IDT_entry
* Inicializa un descriptor de la IDT
*
*Recibe: Puntero a elemento de la IDT
*	 Selector a cargar en el descriptor de interrupcion
*	 Puntero a rutina de atencion de interrupcion	
*	 Derechos de acceso del segmento
*	 Cero
****************************************************************/

void setup_IDT_entry (DESCR_INT *item, byte selector, dword offset, byte access,
			 byte cero) {
  item->selector = selector;
  item->offset_l = offset & 0xFFFF;
  item->offset_h = offset >> 16;
  item->access = access;
  item->cero = cero;
}

/*************************************************************
*initialize_pics
* Inicializa los PICS, el 1 como Master el 2 como slave
* y les coloca los offset enviados
* Recibe: Offset1 para el PIC1
*	  Offset2 para el PIC2
**************************************************************/
void initialize_pics(int offset_pic1, int offset_pic2){
	unsigned char mask1 = _inport(PIC1_DATA);
	unsigned char mask2 = _inport(PIC2_DATA);	

	_outport(PIC1_COMMAND, ICW1);
	_outport(PIC2_COMMAND, ICW1);

	_outport(PIC1_DATA, offset_pic1);
	_outport(PIC2_DATA, offset_pic2);
	
	_outport(PIC1_DATA, 4);
	_outport(PIC2_DATA, 2);

	_outport(PIC1_DATA, ICW4_8086);
	_outport(PIC2_DATA, ICW4_8086);

	_outport(PIC1_DATA, mask1);
	_outport(PIC2_DATA, mask2);
}
