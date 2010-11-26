#include "../include/at_wini.h"


#define BIT0 1
#define BIT1 2
#define BIT2 4
#define BIT3 8
#define BIT4 16
#define BIT5 32
#define BIT6 64
#define BIT7 128
#define BIT8 256
#define BIT9 512
#define BIT10 1024
#define BIT11 2048
#define BIT12 4096
#define BIT13 8192
#define BIT14 16384
#define BIT15 32768

#define IS_REMOVABLE(D) (D & BIT7) ? printf("Is removable\n") : printf("Is not removable\n")
#define IS_ATA_DRIVE(D) (D & BIT15) ? printf("Is not ATA\n") : printf("Is ATA\n")

#define DMA_SUP(D) (D & BIT8) ? printf("DMA is supported\n") : printf("DMA is not supported\n")
#define LBA_SUP(D) (D & BIT9) ? printf("LBA is supported\n") : printf("LBA is not supported\n")
#define DMA_QUEUED_SUP(D) (D & BIT1) ? printf("DMA QUEUED supported\n") : printf("DMA QUEUED is not supported\n")


void init_driver(int ata){

    printf("\n\n========================= \nInicializador de Disco\n========================= \n");

    int status = 0;     // Just to make sure it changes
    status = getStatusRegister(ata);
    if( status & 0x80 )
        printf("Disco ocupado\n");
    if(! ( status & 0x40 ))
        printf("Disco listo, startup (RDY 0)\n");
    printf("Status Register: %d\n", status);

    check_drive(ata);

    if(!(status & BIT3))
        printf("Termine de leer\n");

	printf("MEDIA STATUS\n");
	mediaStatus(ata);
	printf("Status %d\n", getStatusRegister(ata));
    printf("Error %d\n", getErrorRegister(ata));
	



    printf("Ahora voy a intentar leer. Mando un comando primero\n");

    sendDMAcomm(ata);

    printf("Ahora tengo que leer el mensaje de respuesta. Esta en el status register\n");
    status = getStatusRegister(ata);
    printf("Status: %d\n", status);

    if( status & 8 )
        printf("DRQ esta en 1, o sea, tengo informacion en el data register\n");

    if( status & 0x80 ){
        printf("OCUPADO\n");
        return;
    }

    printf("Voy a leer\n");

    //data = getDataRegister(ata);
    //printf("Data: %d\n", data);    

    printf("Status %d\n", getStatusRegister(ata));
    printf("Error %d\n", getErrorRegister(ata));
    printf("\n\n");	
}

unsigned short getStatusRegister(int ata){
	return port_in(ata + WIN_REG7) & 0x00000FFFF;	
}

void identifyDevice(int ata){
	_Cli();
	port_out(ata + WIN_REG6, 0);
	port_out(ata + WIN_REG7, WIN_IDENTIFY);
	_Sti();
}


unsigned short getDataRegister(int ata){
	return portw_in(ata + WIN_REG0) & 0x00000FFFF;
}

void mediaStatus(int ata){
	_Cli();
	port_out(ata + WIN_REG6, 0);
	port_out(ata + WIN_REG7, MEDIA_STATUS);
	_Sti();
}


unsigned short getErrorRegister(int ata){
	return port_in(ata + WIN_REG1) & 0x00000FFFF;
}


void check_drive(int ata){
	printf("-----------------------\n");
	printf("Identifying device...\n");
    identifyDevice(ata);

    int status = getStatusRegister(ata);
    printf("Result: %d\n", status);

    if( status & 8 )
        printf("Data request set. \n");

    unsigned short data = 0;

    int i;
    for(i=0;i<255;i++)    {
        data = getDataRegister(ata);
		switch(i){
			case 0:
				//printf("Data returned (%d): %d\n", i,data);
				IS_REMOVABLE(data);
				IS_ATA_DRIVE(data);
				break;
			case 49:
				DMA_SUP(data);
				LBA_SUP(data);
				break;
			case 83:
				DMA_QUEUED_SUP(data);
				break;
		}
    }

	printf("-----------------------\n");

}


void sendDMAcomm(int ata){
	_Cli();
	port_out(ata + WIN_REG2, 1); // Setea el sector count register en 1
	
	// Setea LBA en 0
	port_out(ata + WIN_REG3, 0);
	port_out(ata + WIN_REG4, 0);
	port_out(ata + WIN_REG5, 0);
	port_out(ata + WIN_REG6, 0x40); // Set LBA bit in 1 and the rest in 0
	port_out(ata + WIN_REG7, READ_DMA); // Set command
	

	_Sti();
	int i = 0;
	for(i = 0; i < 10000; i++)
		__asm__("nop");
}


