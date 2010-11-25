#ifndef _DISKASM_H
#define _DISKASM_H



unsigned short getStatusRegister();

void    identifyDevice();

unsigned short getDataRegister();

void sendDMAcomm();

unsigned short getErrorRegister();

#endif
