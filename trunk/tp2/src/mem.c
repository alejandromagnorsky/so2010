/*
** Task functions file
*/

#include "../include/mem.h"

struct {
    
    pdentry_t (*createEntry) (void*);
    void (*togglePresent) (pdentry_t*);
    
} PageDirectory = { _pd_createEntry,
                    _pd_togglePresent };


pdentry_t _pd_createEntry(void* address) {
    return ((int) address << 12) & 0xFFFFF000;
}

void _pd_togglePresent(pdentry_t* entry) {
    *entry ^= 1;
    
    return;
}
