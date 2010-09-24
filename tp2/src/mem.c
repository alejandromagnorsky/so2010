#include "../include/mem.h"

struct {
    
    pdentry_t (*createEntry) (void*);
    void (*togglePresent) (pdentry_t*);
    
} PageDirectory = { _pd_createEntry,
                    _pd_togglePresent
				};

struct {
    
    void (*start) (pdentry_t*);
    
} Paging = { _startPaging};



pdentry_t _pd_createEntry(void* address) {
    return ((int) address << 12) & 0xFFFFF000;
}

void _pd_togglePresent(pdentry_t* entry) {
    *entry ^= 1;
    
    return;
}


void _startPaging(){
	pdir_t page_directory = _pd_createEmptyDirectoryPage();
	//_setCR3(page_directory);
	page_directory[0] = _kernelTable();
	SET_PRESENT(page_directory[0]);
	_setCR3(page_directory);
	_enablePaging(page_directory);
}

pdir_t _pd_createEmptyDirectoryPage(){
	pdir_t page_directory = PD_DIR_POS;
	int i = 0;
	for(i = 0; i < NENTRIES; i++)
	{
	    page_directory[i] = 0; 
	    SET_PRESENT(page_directory[i], 0);
	    SET_RDWR(page_directory[i]);
	    SET_SUPERVISOR(page_directory[i]);
	}
}

pdir_t _kernelTable(){
	pdir_t kernel_table = KERNEL_TABLE_POS;
	int i = 0;
	for(i = 0; i < NENTRIES; i++){
		page_directory[i] = i * PAGESIZE;
	    SET_PRESENT(page_directory[i], 1);
	    SET_RDWR(page_directory[i], 1);
	    SET_SUPERVISOR(page_directory[i], 1);
	}
}

void _setCR3(pdir_t page_directory){
	asm volatile("mov %0, %%cr3":: "b"(page_directory));
}

void _enablePaging(pdir_t page_directory){
	unsigned int cr0;
	asm volatile("mov %%cr0, %0": "=b"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0":: "b"(cr0));
}

void* _reqpage(void)
{
	
}
