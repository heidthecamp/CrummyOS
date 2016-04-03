/******************************************************************************
 * main: Sample for starting the FAT project.
 *
 * Authors:  Andy Kinley, Archana Chidanandan, David Mutchler and others.
 *           March, 2004.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	int sectPerFat;
	int sectPerTrack;
	int numHead;
	int totSectCount4Fat;
	int bootSig;
	int volID;
    char volLabel[12];
    volLabel[11] = 0;
    char fileSysTyp[9];
    fileSysTyp[9] = 0;

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

	bytesPerSector = getWord(boot, 11);
	sectorsPerCluster = getByte( boot, 13);
	numResSector = getWord( boot, 14 );
	numFat = getByte(boot, 16);
	maxRootDirEnt = getWord(boot, 17);
	totSectCount = getWord(boot, 19);
	sectPerFat = getWord(boot, 22);
	sectPerTrack = getWord(boot, 24);
	numHead = getWord(boot, 26);
	totSectCount4Fat = getDWord(boot, 32);
	bootSig = getByte(boot, 38);
	volID = getDWord(boot, 32);

    memcpy(volLabel, boot+43, 11);
    memcpy(fileSysTyp, boot+54, 8);

	printf("Bytes per Sector           = %d\n", bytesPerSector);
	printf("Sectors per cluster        = %d\n", sectorsPerCluster);
	printf("Number of FATs             = %d\n", numFat);
	printf("Number of reserved sectors = %d\n", numResSector);
	printf("Number of root entries     = %d\n", maxRootDirEnt);
	printf("Total sector count         = %d\n", totSectCount);
	printf("Sectors pre FAT            = %d\n", sectPerFat);
	printf("Sectors per track          = %d\n", sectPerTrack);\
	printf("Number of heads            = %d\n", totSectCount4Fat);
	printf("Boot signature             = 0x%.8x\n", bootSig);
	printf("Volume ID                  = 0x%.8x\n", volID);
    printf("Volume label               = %s\n", volLabel);
    printf("File system type           = %s\n", fileSysTyp);

	return 0;
}
