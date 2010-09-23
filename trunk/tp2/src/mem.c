/*
** Task functions file
*/

#include "../include/mem.h"

struct {
    
    pdentry_t (*createEntry) (void*);
    void (*togglePresent) (pdentry_t*);
    void (*_enablePaging) ();
    
} PageDirectory = { _pd_createEntry,
                    _pd_togglePresent,
                    _enablePaging };


pdentry_t _pd_createEntry(void* address) {
    return ((int) address << 12) & 0xFFFFF000;
}

void _pd_togglePresent(pdentry_t* entry) {
    *entry ^= 1;
    
    return;
}

void _enablePaging()
{
	_setPG ( _read_cr0() | 0x80000000 );
}
