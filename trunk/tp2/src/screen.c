#include "../include/screen.h"

size_t video_write(device_t dev, void* from, size_t nbytes){
	size_t wpos;
	char zero = 0;
	int colpos;
	unsigned char format = 0x07;
	int i;

	if(nbytes != 1)
		return 0;
	
	unsigned char ascii = *((char*) from);

	switch(ascii){
		case '\t':
			if(dev->wpos % 8 == 0)
			  for(i=0; i<4; i++)
			    System.write(dev->id, &zero, 1);
			while(dev->wpos % 8 > 0)
			    System.write(dev->id, &zero, 1);
			break;
		case '\n':
			wpos = dev->wpos;
			if(wpos >= 3840){
				scroll(dev);
			}else{
				colpos = wpos % 160;
				for( i = colpos; i < 160; i++){
					if(i % 2 == 0)						
						System.write(DEVICE_SCREEN, &zero, 1);
				}
			}
			break;
		case '\b':
			if(dev->wpos != 0){
				*((char*)dev->addr + --dev->wpos) = 0x07;
				*((char*)dev->addr + --dev->wpos) = 0;
			}
			break;
		default:
			if(dev->wpos == 4000){
				scroll(dev);
			}
			*((char*)dev->addr + dev->wpos++) = ascii;
			*((char*)dev->addr + dev->wpos++) = 0x07;
	}
	move_cursor(dev->wpos/2);
	return nbytes;
}

void move_cursor_rc(int row, int col){
	move_cursor((row*80) + col);
}

void move_cursor(int position){
	unsigned short p1 = 0x3D4;
	unsigned short p2 = 0x3D5;

	unsigned char low= position & 0xFF ;
	unsigned char high = position >> 8;

	unsigned char exp1 = 0x0E;
	unsigned char exp2 = 0x0F;
 
	//Out a p1 con 0x0E y pasa la parte alta.	
	__asm__("outb %1,%0":: "dN" (p1), "a"(exp1));
	__asm__("outb %1,%0"::"dN" (p2), "a"(high));
	//Out a p2 con 0x0F y pasa la parte baja.
	__asm__("outb %1,%0"::"dN" (p1), "a"(exp2));
	__asm__("outb %1,%0"::"dN" (p2), "a"(low));
}


void scroll(device_t dev){
	int zero = 0;
	int i = 0;
	for( i = 0; i < 24; i++){
		_memcpy(dev->addr + i*160 + 160, dev->addr + i*160, 160);
	}
	
	dev->wpos = 3840;
	while(dev->wpos < 4000)
		System.write(dev->id, &zero, 1);
	dev->wpos = 3840;
}
