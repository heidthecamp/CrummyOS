#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "boot.h"
#include "fatSupport.h"


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
    else
        printf("\nSHM segment has been found.\n");

    if ((pointer = shmat(shm_ID, NULL, 0)) == NULL){
        perror("Error including SHM address space.");
        exit(0);
    }
    else
        printf("Allocating SHM to my address space.\n");
    printf("Copying message into shaged memory...\n");
    memcpy(pointer, &shrmem, sizeof(struct SharedMemory));

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


int getWord( unsigned char* buf, int index ){

    int MSB, LSB, word;
    // 12 (not 11) because little endian
    MSB  = ( ( (int) buf[index + 1] ) << 8 ) & 0x0000ff00;
    LSB  =   ( (int) buf[index] )        & 0x000000ff;
    word = MSB | LSB;
    return word;

}


int getByte( unsigned char* buf, int index ){

	int byte;
	byte = ( (int) buf[index] );

	return byte;

}

int main(int argc, char const **argv) {

    char * file;
    char * mode;

    if (argv[1] == NULL)
    {
        printf("Boot needs at lest 1 argument\n");
        exit(EXIT_FAILURE);
    } else
    {
        file = argv[1];
    }

    if (argv[2] == NULL) {
        mode = "r";
    } else
    {
        mode = argv[2];
    }

    //memcpy(shrmem.fileSysTyp, buf + 54, 8);
    memcpy(&shrmem.mode, &mode, sizeof(char *));
    FILE_SYSTEM_ID = fopen(file, mode);
    setFSID(FILE_SYSTEM_ID);

    shl_shareMemoryGet();


    unsigned char* buf;
    buf = malloc(BYTES_TO_READ_IN_BOOT_SECTOR);

    setBPS(BYTES_TO_READ_IN_BOOT_SECTOR);


    if (read_sector(0, buf) == -1){
        printf("Something has gone wrong -- could not read the shrmem sector\n");
    }

    shrmem.bytesPerSector = getWord(buf, 11);
    setBPS(shrmem.bytesPerSector);
    shrmem.sectorsPerCluster = getByte(buf, 13);
    shrmem.numResSector = getWord(buf, 14 );
    shrmem.numFat = getByte(buf, 16);
    shrmem.maxRootDirEnt = getWord(buf, 17);
    shrmem.totSectCount = getWord(buf, 19);
    shrmem.sectPerFat = getWord(buf, 22);
    shrmem.sectPerTrack = getWord(buf, 24);
    shrmem.numHead = getWord(buf, 26);
    shrmem.totSectCount4Fat = getDWord(buf, 32);
    shrmem.bootSig = getByte(buf, 38);
    shrmem.volID = getDWord(buf, 39);


    shrmem.volLabel[11] = 0;
    shrmem.fileSysTyp[8] = 0;
    memcpy(shrmem.volLabel, buf + 43, 11);
    memcpy(shrmem.fileSysTyp, buf + 54, 8);

    free(buf);


    shrmem.fildes = fileno(FILE_SYSTEM_ID);

    shl_shareMemorySet();

    return 0;
}
