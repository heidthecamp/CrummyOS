#include "pbs.h"
#include "boot.h"


int main( int argc, char **argv ){

    boot_t boot;
	// You must set two global variables for the disk access functions:
	//      FILE_SYSTEM_ID         BYTES_PER_SECTOR



	// Use this for an image of a floppy drive
    FILE_SYSTEM_ID = fopen("floppy1", "r+");
    setFSID(FILE_SYSTEM_ID);

    if (FILE_SYSTEM_ID == NULL)
    {
        printf("Could not open the floppy drive or image.\n");
        exit(1);
    }

	// Set it to this only to read the boot sector
	// BYTES_PER_SECTOR = BYTES_TO_READ_IN_BOOT_SECTOR;

	// Then reset it per the value in the boot sector


    boot = readBootSector();


    printf("Bytes per Sector           = %d\n", boot.bytesPerSector);
    printf("Sectors per cluster        = %d\n", boot.sectorsPerCluster);
    printf("Number of FATs             = %d\n", boot.numFat);
    printf("Number of reserved sectors = %d\n", boot.numResSector);
    printf("Number of root entries     = %d\n", boot.maxRootDirEnt);
    printf("Total sector count         = %d\n", boot.totSectCount);
    printf("Sectors pre FAT            = %d\n", boot.sectPerFat);
    printf("Sectors per track          = %d\n", boot.sectPerTrack);\
    printf("Number of heads            = %d\n", boot.totSectCount4Fat);
    printf("Boot signature             = 0x%.8x\n", boot.bootSig);
    printf("Volume ID                  = 0x%.8x\n", boot.volID);
    printf("Volume label               = %s\n", boot.volLabel);
    printf("File system type           = %s\n", boot.fileSysTyp);

    return 0;
}
