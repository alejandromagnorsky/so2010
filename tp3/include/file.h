

#ifndef FILE_H_
#define FILE_H_

// Very simple file interface
// For now, just use contiguous sectors for files, no fragmentation ( no sector nodes )


#define _MAX_FILES 500
#define _DEFAULT_FILESIZE 512
#define _FILE_NAMELENGTH 255

#define _FILETABLE_SECTOR_START 20

#define _FILE_HEADER "FILE_START"
#define _FILE_FOOTER "FILE_END"

// Estupido malloc
#define _FILE_CHILDREN 100

#define _FILE_ROOT "%"


#define EOF -1


// Previous typedef for recursive declaration.
typedef struct File File;

struct File {
	char name[_FILE_NAMELENGTH];
	unsigned int sector;	// This should be replaced with fileNode later
	unsigned int psector;	// Parent sector, to simplify things
	unsigned int size;	// Size, NOT considering header
	File * parent;
	File * children[_FILE_CHILDREN];
};


// We need 10 MB, which is 19531 sectors. So we allocate 15 k sectors to be safe.
// With a bitmap, we only ned 1875 chars to do this.
// This means i need only 4 sectors to allocate the sector table

#define _MAX_SECTORS 15000
#define _MAX_SECTOR_BYTES (_MAX_SECTORS/8)

// Sector table position
#define _TABLE_STARTUP_SECTOR 10 
#define _TABLE_SECTOR_SIZE (_MAX_SECTOR_BYTES/512)

#define _TABLE_HEADER "TABLE START"

extern char sectorTable[_MAX_SECTOR_BYTES];

extern File fileTable[_MAX_FILES];


void createFilesystem();

int loadFileSystem();

int fileIO(char * a);


int touch( char * arg );

int ls(char * a);

int tree(char *a);

#endif
