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

void sendComm(int ata, int rdwr, unsigned short sector);
void read(int ata, char * ans, unsigned short sector, int offset, int count);
unsigned short getDataRegister(int ata);
void write(int ata, char * msg, int bytes, unsigned short sector, int offset);
void writeDataToRegister(int ata, char upper, char lower);
void translateBytes(char ans[], unsigned short sector);

void init_driver(int ata){

	printf("\n\n========================= \nInicializador de Disco\n========================= \n");

	int status = 0;     // Just to make sure it changes;

	/* // Prints no los borro por las dudas
	status = getStatusRegister(ata);
	if( status & 0x80 )
	printf("Disco ocupado\n");
	if(! ( status & 0x40 ))
	printf("Disco listo, startup (RDY 0)\n");
	printf("Status Register: %d\n", status);

	printf("Ahora voy a intentar leer. Mando un comando primero\n");


	printf("Ahora tengo que leer el mensaje de respuesta. Esta en el status register\n");
	status = getStatusRegister(ata);
	printf("Status: %d\n", status);

	if( status & 8 )
	printf("DRQ esta en 1, o sea, tengo informacion en el data register\n");

	if( status & 0x80 ){
	printf("OCUPADO\n");
	return;
	}*/

	check_drive(ata);

	/* TEST READ */
	printf("\n------------------\nTEST READ\n------------------\n");
	char * r = (char *)malloc(512);
/*
	read(ata, r, 0, 0, 18);
	printf("Primera parte usando offset 0 cantidad 18:\n%s\n", r);

	read(ata, r, 0, 18, 20);
	printf("Segunda parte usando offset 18 cantidad 20:\n%s\n", r);

	/* ACLARACION POR SI LES SURGIO LA DUDA:
	 	+ En la salida en el bochs de los 2 printf de arriba se come la letra 'a' de 'apartments'
	 	+ Eso pasa porque como siempre levanto de a 2 bytes, es "o levanto los 2 bytes" o "no levanto ninguno".
	 	+ La letra 'a' debe caer en el medio de eso y por eso redondea para abajo el algoritmo y no la toma.
	 	+ Si el primer read lo cambian por:
	 				+ read(ata, ans, 0, 0, 19);
	 		les va a leer los 2 pedazos bien divididos (donde termina uno empieza el otro). 
	 	
	 	+ TIENE SOLUCION, NO ES DIFICIL, EN VEZ DE REDONDEAR PARA ABAJO (PEDIR DE MENOS) HAY QUE REDONDEAR PARA ARRIBA
	 		(PEDIR DE MÁS) PERO ME DA PAJA HACERLA AHORA.
	*/

	/* TEST WRITE */
	printf("\n------------------\nTEST WRITE\n------------------\n");


	int sec = 10;

	read(ata, r, sec,0 , 512);
	printf("El sector %d esta asi antes de leer:\n%s\n\n", sec, r);
	printf("Ahora esta asi:\n\n");
	char * msg = (char*)malloc(512);
	int i;
	for(i=0;i<512;i++)
		msg[i] = 'G';

	write(ata, msg, 50, sec, 37);	// Por ahora con write no andan offset ni sector

	read(ata, r, sec, 0, 50);

	printf("%s\n", r);    
/*
	printf("Status: %d\n", getStatusRegister(ata));
	printf("Error: %d\n", getErrorRegister(ata));*/
}

// To read N bytes from hard disk, must alloc N+1 bytes for ans, as N+1 byte is used to null-character
void read(int ata, char * ans, unsigned short sector, int offset, int count){
	int b;
	unsigned short data;
	int size = sizeof(unsigned short);
	sendComm(ata, READ, sector);

	// Discards useless data until offset byte is reached
	for (b=0; b < offset/2; b++)
		getDataRegister(ata);

	b = 0;
	while( b < count-1 ){
		data = getDataRegister(ata);
		translateBytes(ans + (b++ * size), data);
	}

	ans[count-1] = 0;
}

void translateBytes(char * ans, unsigned short databyte){	
	ans[0] = databyte & 0xFF;
	ans[1] = databyte >> 8;
}

void write(int ata, char * msg, int bytes, unsigned short sector, int offset){

	// If it won't enter on sector, return
	if(bytes > 512-offset )
		return;

	// Save sector data
	char * tmp = (char*) malloc(512);
	read(ata,tmp,sector,0,512);

	// Send write command
	sendComm(ata, WRITE, sector);

	// Prepare sector with new data
	int c;
	for(c=0;c<bytes;c++)
		tmp[offset+c] = msg[c];

	// Now write all sector
	int b;
	for(b=0;b<=512;b+=2)
		writeDataToRegister(ata, tmp[b+1], tmp[b]);
}

void writeDataToRegister(int ata, char upper, char lower){
	_Cli();
	unsigned short out;
	
	// Wait for driver's ready signal.
	while (!(portw_in(ata + WIN_REG7) & BIT3));
	
	out = (upper << 8) | lower;
	portw_out(ata + WIN_REG0, out);
	
	_Sti();
}

unsigned short getDataRegister(int ata){
	_Cli();
	unsigned short ans;
	
	// Wait for driver's ready signal.
	while (!(portw_in(ata + WIN_REG7) & BIT3));

	ans = portw_in(ata + WIN_REG0);
	
	_Sti();
	return ans;
}

unsigned short getErrorRegister(int ata){
	_Cli();
	unsigned short rta = port_in(ata + WIN_REG1) & 0x00000FFFF;
	_Sti();
	return rta;
}

void sendComm(int ata, int rdwr, unsigned short sector){
	_Cli();
	
	port_out(ata + WIN_REG1, 0);
	port_out(ata + WIN_REG2, 0);	// Set count register sector in 1
	
	port_out(ata + WIN_REG3, (unsigned char)sector);			// LBA low
	port_out(ata + WIN_REG4, (unsigned char)(sector >> 8));		// LBA mid
	port_out(ata + WIN_REG5, (unsigned char)(sector >> 16));	// LBA high
	port_out(ata + WIN_REG6, 0xE0 | (ata << 4) | ((sector >> 24) & 0x0F));	// Set LBA bit in 1 and the rest in 0
	
	// Set command
	if(rdwr == READ)
		port_out(ata + WIN_REG7, LBA_READ);
	else
		port_out(ata + WIN_REG7, LBA_WRITE);

	_Sti();
}

unsigned short getStatusRegister(int ata){
	unsigned short rta;
	_Cli();
	rta = port_in(ata + WIN_REG7) & 0x00000FFFF;
	_Sti();
	return rta;
}

void identifyDevice(int ata){
	_Cli();
	port_out(ata + WIN_REG6, 0);
	port_out(ata + WIN_REG7, WIN_IDENTIFY);
	_Sti();
}

void check_drive(int ata){
	printf("-----------------------\n");
	printf("Identifying device...\n");
    identifyDevice(ata);

    int status = getStatusRegister(ata);
    printf("Status: %d\n", status);

    unsigned short data = 0;

    int i;
    for(i=0;i<255;i++){
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

