/******************************************************************************
 * main: Sample for starting the FAT project.
 *
 * Authors:  Andy Kinley, Archana Chidanandan, David Mutchler and others.
 *           March, 2004.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

// 13 is NOT the correct number -- you fix it!
#define BYTES_TO_READ_IN_BOOT_SECTOR 62

/******************************************************************************
 * You must set these global variables:
 *    FILE_SYSTEM_ID -- the file id for the file system (here, the floppy disk
 *                      filesystem)
 *    BYTES_PER_SECTOR -- the number of bytes in each sector of the filesystem
 *
 * You may use these support functions (defined in FatSupport.c)
 *    read_sector
 *    write_sector
 *    get_fat_entry
 *    set_fat_entry
 *****************************************************************************/

FILE* FILE_SYSTEM_ID;
int BYTES_PER_SECTOR;

extern int read_sector(int sector_number, char* buffer);
extern int write_sector(int sector_number, char* buffer);

extern int  get_fat_entry(int fat_entry_number, char* fat);
extern void set_fat_entry(int fat_entry_number, int value, char* fat);

/******************************************************************************
 * main: an example of reading an item in the boot sector
 *****************************************************************************/

int getByte( unsigned char* buf, int index ){

	int byte;
	byte = ( (int) buf[index] );

	return byte;

}

int getWord( unsigned char* buf, int index ){

	int MSB, LSB, word;
	// 12 (not 11) because little endian
	MSB  = ( ( (int) buf[index + 1] ) << 8 ) & 0x0000ff00;
	LSB  =   ( (int) buf[index] )        & 0x000000ff;
	word = MSB | LSB;
	return word;

}

int getDWord( unsigned char* buf, int index ){

	int b0, b1, b2, b3, dword;

	b3 = ( ( (int) buf[index + 3] ) << 24 ) & 0xff000000;
	b2 = ( ( (int) buf[index + 2] ) << 16 ) & 0x00ff0000;
	b1 = ( ( (int) buf[index + 1] ) << 8  ) & 0x0000ff00;
	b0 =   ( (int) buf[index]     )         & 0x000000ff;
	dword = b3 | b2 | b1 | b0;
	return dword;

}

int main()
{
	unsigned char* boot;            // example buffer

	int mostSignificantBits;
	int leastSignificantBits;
	int bytesPerSector;
	int sectorsPerCluster;
	int numResSector;
	int numFat;
	int maxRootDirEnt;
	int totSectCount;
	int ignore;
	int sectPerFat;
	int sectPerTrack;
	int numHead;
	int totSectCount4Fat;
	int bootSig;
	int volID;

	// You must set two global variables for the disk access functions:
	//      FILE_SYSTEM_ID         BYTES_PER_SECTOR

	// Use this for an image of a floppy drive
	FILE_SYSTEM_ID = fopen("floppy1", "r+");

	if (FILE_SYSTEM_ID == NULL)
	{
		printf("Could not open the floppy drive or image.\n");
		exit(1);
	}

	// Set it to this only to read the boot sector
	BYTES_PER_SECTOR = BYTES_TO_READ_IN_BOOT_SECTOR;

	// Then reset it per the value in the boot sector

	boot = (unsigned char*) malloc(BYTES_PER_SECTOR * sizeof(unsigned char));

	if (read_sector(0, boot) == -1)
		printf("Something has gone wrong -- could not read the boot sector\n");


	// 12 (not 11) because little endian
	mostSignificantBits  = ( ( (int) boot[12] ) << 8 ) & 0x0000ff00;
	leastSignificantBits =   ( (int) boot[11] )        & 0x000000ff;
	bytesPerSector = getWord(boot, 11);

	printf("Bytes per Sector \t\t =%d\n", bytesPerSector);

	sectorsPerCluster = getByte( boot, 13);

	printf("Sectors per cluster \t\t =%d\n", sectorsPerCluster);

	numFat = getByte(boot, 16);

	printf("Number of FATs \t\t\t =%d\n", numFat);

	numResSector = getWord( boot, 14 );

	printf("Number of reserved sectors \t =%d\n", numResSector);


	maxRootDirEnt = getWord(boot, 17);

	printf("%d\n", maxRootDirEnt);

	totSectCount = getWord(boot, 19);

	printf("%d\n", totSectCount);

	sectPerFat = getWord(boot, 22);

	printf("%d\n", sectPerFat);

	sectPerTrack = getWord(boot, 24);

	printf("%d\n", sectPerTrack);

	numHead = getWord(boot, 26);

	printf("%d\n", sectPerTrack);

	totSectCount4Fat = getDWord(boot, 32);

	printf("%d\n", totSectCount4Fat);

	bootSig = getByte(boot, 38);

	printf ("0x%.8x\n", bootSig);

	volID = getDWord(boot, 32);

	printf("0x%.8x\n", volID);





	return 0;
}
