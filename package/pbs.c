#include "pbs.h"
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void shl_shareMemoryGet(){
    int shm_ID = 0;
    void *pointer = NULL;
    if ((shm_ID = shmget(SHMKEY, SHMSIZE, 0444)) < 0 )
    {
        perror("Error getting SHM segment.");
        exit(-1);
    }
    if ((pointer = shmat(shm_ID, NULL, 0)) == NULL)
    {
        perror("Error including SHM address space.");
        exit(0);
    }
    memcpy(&shrmem, pointer, sizeof(struct SharedMemory));

}


void shl_shareMemorySet(){
    int shm_ID = 0;
    void *pointer = NULL;

    if ((shm_ID = shmget(SHMKEY, SHMSIZE, 0666)) < 0)
    {
        perror("Error getting SHM segment.");
        exit(-1);
    }

    if ((pointer = shmat(shm_ID, NULL, 0)) == NULL){
        perror("Error including SHM address space.");
        exit(0);
    }
    memcpy(pointer, &shrmem, sizeof(struct SharedMemory));

}


int main( int argc, char **argv ){
    /*
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


    //boot = readBootSector();
*/
    shl_shareMemoryGet();

    printf("Bytes per Sector           = %d\n", shrmem.bytesPerSector);
    printf("Sectors per cluster        = %d\n", shrmem.sectorsPerCluster);
    printf("Number of FATs             = %d\n", shrmem.numFat);
    printf("Number of reserved sectors = %d\n", shrmem.numResSector);
    printf("Number of root entries     = %d\n", shrmem.maxRootDirEnt);
    printf("Total sector count         = %d\n", shrmem.totSectCount);
    printf("Sectors pre FAT            = %d\n", shrmem.sectPerFat);
    printf("Sectors per track          = %d\n", shrmem.sectPerTrack);\
    printf("Number of heads            = %d\n", shrmem.totSectCount4Fat);
    printf("Boot signature             = 0x%.8x\n", shrmem.bootSig);
    printf("Volume ID                  = 0x%.8x\n", shrmem.volID);
    printf("Volume label               = %s\n", shrmem.volLabel);
    printf("File system type           = %s\n", shrmem.fileSysTyp);

    return 0;
}
