#include "../include/file.h"
#include "../include/at_wini.h"
#include "../include/klib.h"


File fileTable[_MAX_FILES];
char sectorTable[_MAX_SECTOR_BYTES] = {0};


void __initFileTable();

void __printFile(File * f);

void __printFileTreeTabs( File *f, int tabs);


void __loadFileTable();

void __addFileChild(File * parent, File * child);


void clearSector(int sector){
	char bit = sector%8;
	sectorTable[sector/8] &= ~(1 << bit);
	return;
}

void setSector(int sector){
	char bit = sector%8;
	sectorTable[sector/8]|= 1 << bit;
	return;
}

int getSector(int sector){
	char bit = sector%8;
	return sectorTable[sector/8] & (1<<bit);
}


void exportTable(){
	disk_cmd cmd = {ATA0, _TABLE_STARTUP_SECTOR, 0, _MAX_SECTOR_BYTES };
	cmd.buffer = sectorTable;
	System.writeDisk(&cmd);
}


void createTable(){
	int i;

	// First chunk is protected
	for(i=0;i<_TABLE_STARTUP_SECTOR;i++)
		setSector(i);

	// Sector table is protected
	for(i=0;i<_TABLE_SECTOR_SIZE;i++)
		setSector(_TABLE_STARTUP_SECTOR+i);
	exportTable();

	__initFileTable();
}

void loadTable(){
	disk_cmd cmd = {ATA0, _TABLE_STARTUP_SECTOR, 0, _MAX_SECTOR_BYTES };
	cmd.buffer = sectorTable;
	System.readDisk(&cmd);

	__loadFileTable();
}

void printTable(int from, int to){
	int i;
	for(i=from;i<to;i++){
		int exists = getSector(i);
		printf("Sector %d: %s\n", i, exists ? "exists" : "does not exist");
	}
}	

int __getFileFreeSector(){
	int i;
	for(i=_FILETABLE_SECTOR_START;i<_MAX_SECTORS;i++)
		if(!getSector(i))
			return i;
	return -1;
}

int getFreeSector(){
	int i;
	for(i=0;i<_MAX_SECTORS;i++)
		if(!getSector(i))
			return i;
	return -1;
}

void __freeFile(int index){
	fileTable[index].sector = -1;
}

File * getFreeFile(){
	int i;
	for(i=0;i<_MAX_FILES;i++)
		if(fileTable[i].sector == -1)
			return fileTable+i;
	return NULL;
}

File * __loadFile(int sector){
	// If it isnt mapped yet...
	if(!getSector(sector))
		return NULL;

	File * file = getFreeFile();


	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		file->children[i] = NULL;
	

	file->sector = sector;

	// FILE HEADER
	// 255 bytes: name
	// 4 bytes: size
	// 4 bytes: parent sector
	// FILE FOOTER

	int base = 0;

	char header[50];
	char footer[50];

	// Now prepare to read file header on sector
	disk_cmd fileHeader = {ATA0, file->sector, base, strlen(_FILE_HEADER)+1,header};
	base += strlen(_FILE_HEADER)+1;

	disk_cmd fileName = {ATA0, file->sector, base, _FILE_NAMELENGTH, file->name};
	base += _FILE_NAMELENGTH;

	disk_cmd fileSize = {ATA0, file->sector,base, 4,(char*)(&(file->size))};
	base += sizeof(int);

	disk_cmd fileParent = {ATA0, file->sector, base, sizeof(int), (char *)(&file->psector)};
	base += sizeof(int);

	file->parent = NULL;

	disk_cmd fileFooter = {ATA0, file->sector, base, strlen(_FILE_FOOTER)+1,footer};

	// And read it
	System.readDisk(&fileHeader);
	System.readDisk(&fileName);
	System.readDisk(&fileSize);
	System.readDisk(&fileParent);
	System.readDisk(&fileFooter);

	// Its a file
	if(!strcmp(header,_FILE_HEADER) && !strcmp(footer,_FILE_FOOTER))
		return file;

	// File not recognized
	__freeFile(sector);
	return NULL;
}


// Allocate a file on disk!
void __allocateFile(File * file){
	// Set sector on sectorTable
	setSector(file->sector);

	// And set all necessary sectors for file
	int sectors = (file->size / 512) + 1;
	int i;
	for(i=0;i<sectors;i++)
		setSector(file->sector+i);

	// Update table on disk, not that expensive
	exportTable();

	// FILE HEADER
	// 255 bytes: name
	// 4 bytes: size
	// 4 bytes: parent sector
	// FILE FOOTER

	int base = 0;

	disk_cmd fileHeader = {ATA0, file->sector, base, strlen(_FILE_HEADER)+1,_FILE_HEADER};
	base += strlen(_FILE_HEADER)+1;

	// Now prepare file header on sector
	disk_cmd fileName = {ATA0, file->sector, base, _FILE_NAMELENGTH, file->name};
	base += _FILE_NAMELENGTH;

	disk_cmd fileSize = {ATA0, file->sector,base, sizeof(int), (char*)(&(file->size))};
	base += sizeof(int);

	int parentSector = file->parent != NULL ? file->parent->sector : -1;
	disk_cmd fileParent = {ATA0, file->sector, base, sizeof(int), (char *)(&parentSector)};
	base += sizeof(int);

	disk_cmd fileFooter = {ATA0, file->sector, base, strlen(_FILE_FOOTER)+1, _FILE_FOOTER };

	// And allocate it
	System.writeDisk(&fileHeader);
	System.writeDisk(&fileName);
	System.writeDisk(&fileSize);
	System.writeDisk(&fileParent);
	System.writeDisk(&fileFooter);
}


void __printFile(File * f){
	printf("Filename: %s\n", f->name);
	printf("Sector: %d\n", f->sector);
	printf("Size: %d\n", f->size);
	printf("Parent: %s\n", f->parent != NULL ? f->parent->name : "null");
}

void __initFileName(File * f){
	int i;
	for(i=0;i<_FILE_NAMELENGTH;i++)
		f->name[i] = 0;
}

File * __createFile(char * name, int sector, int size, File * parent){
	File * out = getFreeFile();

	// Prepare name
	__initFileName(out);
	_memcpy(name, out->name, strlen(name)+1);

	out->sector = sector;
	out->size = size;
	out->parent = parent;
	if(parent != NULL){
		out->psector = parent->sector;
		__addFileChild(parent,out);
	}
	else out->psector = -1;

	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		out->children[i] = NULL;
	
	return out;
}


File * __getFileBySector(int sector){
	if(sector == -1 )
		return NULL;

	int i;
	for(i=0;i<_MAX_FILES;i++)
		if(fileTable[i].sector == sector)
			return fileTable+i;
}


void __addFileChild(File * parent, File * child){
	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		if(parent->children[i] == NULL){
			parent->children[i] = child;
			return;
		}
}


void __constructFileTree(){

	int i;
	for(i=0;i<_MAX_FILES;i++)
		if(fileTable[i].sector != -1){

			File * f = fileTable+i;

			// Get parent and construct tree
			File * parent = __getFileBySector(f->psector);
			if(parent != NULL){

				// Add parent to child
				f->parent = parent;

				// Add child to parent
				__addFileChild(parent, f);

			}
		}
}


// Just load it, then do processing
void __loadFileTable(){
	int i;
	for(i=0;i<_MAX_FILES;i++)
		fileTable[i].sector = -1;
	
	for(i=_FILETABLE_SECTOR_START;i<_MAX_SECTORS;i++)
		if(getSector(i)){
			// Try to load file
			File * file = __loadFile(i);
		}

	// Now process and construct tree
	__constructFileTree();
}

void __printFileTree(File * f){
	__printFileTreeTabs(f, 0);
}


void __printChildren(File *f){
	int i;
	for(i=0;i<_FILE_CHILDREN;i++)
		if(f->children[i]!=NULL)
			printf("%s\n", f->children[i]->name);

}

void __printFileTreeTabs( File *f, int tabs){
	int i;
	for(i=0;i<tabs;i++)
		if(i)
			printf(" |\t\t");
		else printf(" \t\t");

	if(tabs)
		printf(" |-");

	printf("%s",f->name);

	int hasChilds = 0;

	for(i=0;i<_FILE_CHILDREN;i++)
		if(f->children[i] != NULL){

			// Print it nicely
			if(!hasChilds){
				hasChilds = 1;
				printf("\n");
			}

			__printFileTreeTabs(f->children[i], tabs+1);
		}

	if(!hasChilds) {
		printf("\n");
	}

}


void __initFileTable(){
	int i;
	for(i=0;i<_MAX_FILES;i++)
		fileTable[i].sector = -1;

	File * base = __createFile("/",__getFileFreeSector(),_DEFAULT_FILESIZE, NULL);
	__allocateFile(base);


	File * usr = __createFile("usr",__getFileFreeSector(),_DEFAULT_FILESIZE, base);
	__allocateFile(usr);

	File * bin = __createFile("bin", __getFileFreeSector(), _DEFAULT_FILESIZE,base);
	__allocateFile(bin);

	File * dev = __createFile("dev", __getFileFreeSector(), _DEFAULT_FILESIZE,base);
	__allocateFile(dev);

	File * home = __createFile("home", __getFileFreeSector(), _DEFAULT_FILESIZE,base);
	__allocateFile(home);
}


/*

	char * buffer = (char *) malloc(512);

	disk_cmd cmd = {ATA0, 40, 0, 512, buffer};
	System.readDisk(&cmd);

	cmd.buffer[511] = '\0';

	printf("%s\n", cmd.buffer);
}

int write_disk(char * a){
	disk_cmd cmd = {ATA0, 40, 0, strlen(a)};
	cmd.buffer = a;
	System.writeDisk(&cmd);

*/

int fileIO(char * a){
	printf("Vamos a crear la tabla\n");
	printf("Que hago?\n1) crear tabla\n2) cargar tabla\n");
	int opt = 0;
	scanf("%d",&opt);
	printf("\n");

	if(opt == 1)
		createTable();
	else if(opt== 2)
		loadTable();

	File * f = fileTable;

	printf("\nFile tree: \n");
	__printFileTree(f);

}
