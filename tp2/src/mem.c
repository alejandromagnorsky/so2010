#include "../include/mem.h"

/* Private memory functions */
void _pd_createDirectoryTbl();
void _setCR3();
void _enablePaging();
void _setEntry(pentry_t * entry, int address);

/* Directory Table */
static ptbl_t directoryTbl;


/* Namespace structure */
struct {
	void (*start)();
    pentry_t (*createEntry) (void*);
    void (*togglePresent) (pentry_t*);
} Paging = {_startPaging,
			_pd_createEntry,
			_pd_togglePresent
		};



void _setEntry(pentry_t * entry, int address){
	address &= 0xFFFFF000;
	*entry = address;
}

pentry_t _pd_createEntry(void* address) {
    return ((int) address << 12) & 0xFFFFF000;
}

void _pd_togglePresent(pentry_t * entry) {
    *entry ^= 1;
    return;
}


void _startPaging(){
	_pd_createDirectoryTbl();
	_setCR3();
	_enablePaging();
}

void _pd_createDirectoryTbl(){
	directoryTbl = (ptbl_t) KERNEL_AREA - PAGESIZE;
	ptbl_t tbl;
	int i = 0, j = 0;
	address_t address = 0;
	
	for(i = 0; i < NENTRIES; i++)
	{
		tbl = (ptbl_t) KERNEL_AREA + i * PAGESIZE;
		_setEntry(&directoryTbl[i], (address_t) tbl);
		i == 0 ? (directoryTbl[i] |= 0x03) : (directoryTbl[i] |= 0x02);
		directoryTbl[i] |= 0x02;
		for(j = 0; j < NENTRIES; j++){
			_setEntry(&tbl[j], address);
			i == 0 ? (tbl[j] |= 0x03) : (tbl[j] |= 0x02);
			address += PAGESIZE;
		}
	}
}


void _setCR3(){
	//Set the page_directory address in CR3
	asm volatile("MOVL 	%0, %%CR3" : : "b" (directoryTbl));
}

void _enablePaging(){
	asm volatile("MOVL	%CR0, %EAX"); 			//Get the value of CR0.
	asm volatile("OR		$0x80000000, %EAX");// Set PG bit.
	asm volatile("MOVL 	%EAX, %CR0");			//Set CR0 value.
}

void* _reqpage(void)
{
	
}

