#include "../include/at_wini.h"

void init_driver(){

    printf("\n\n========================= \nInicializador de Disco\n========================= \n");


    int status = 0;     // Just to make sure it changes
    status = getStatusRegister();

    if( status & 0x80 )
        printf("Disco ocupado\n");

    if(! ( status & 0x40 ))
        printf("Disco listo, startup (RDY 0)\n");

    printf("Status Register: %d\n", status);

    printf("Identifying device...\n");
    identifyDevice();

    status = getStatusRegister();
    printf("Result: %d\n", status);

    if( status & 8 )
        printf("Data request set. \n");

    unsigned short data = 0;

    int i;
    for(i=0;i<255;i++)    {
        data = getDataRegister();
        //printf("Data returned (%d): %d\n", i,data);
    }

    status = getStatusRegister();

    if(!(status & 8))
        printf("Termine de leer\n");


    printf("Ahora voy a intentar leer. Mando un comando primero\n");

    sendDMAcomm();

    printf("Ahora tengo que leer el mensaje de respuesta. Esta en el status register\n");

    status = getStatusRegister();

    printf("Status: %d\n", status);

    if( status & 8 )
        printf("DRQ esta en 1, o sea, tengo informacion en el data register\n");

    if( status & 0x80 ){
        printf("OCUPADO\n");
        return;
    }

    printf("Voy a leer\n");

    data = getDataRegister();
    printf("Data: %d\n", data);    

    printf("Status %d\n", getStatusRegister());
    printf("Error %d\n", getErrorRegister());
    printf("\n\n");	
}

unsigned short getStatusRegister(){
	return port_in(WIN_REG7) & 0x00000FFFF;	
}

void	identifyDevice(){
	_Cli();
	port_out(WIN_REG6, 0);
	port_out(WIN_REG7, WIN_IDENTIFY);
	_Sti();
}


unsigned short getDataRegister(){
	return portw_in(WIN_REG0) & 0x00000FFFF;
}

unsigned short getErrorRegister(){
	return port_in(WIN_REG1) & 0x00000FFFF;
}


void sendDMAcomm(){
	_Cli();
	port_out(WIN_REG2, 1); // Setea el sector count register en 1
	
	// Setea LBA en 0
	port_out(WIN_REG3, 0);
	port_out(WIN_REG4, 0);
	port_out(WIN_REG5, 0);
	port_out(WIN_REG6, 0x40); // Set LBA bit in 1 and the rest in 0
	port_out(WIN_REG7, USE_DMA); // Set command
	
	int i = 0;
	for(i = 0; i < 100000; i++)
		__asm__("nop");

	_Sti();
}


