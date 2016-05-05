#ifndef BOOT_H
#define  BOOT_H
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define BYTES_TO_READ_IN_BOOT_SECTOR 62


#define SHMKEY  2113
#define SHMSIZE 500


struct SharedMemory
{
    int fildes;
    char * mode;
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
    char fileSysTyp[9];
};

struct SharedMemory shrmem;

FILE* FILE_SYSTEM_ID;

void shl_shareMemoryGet();
void shl_shareMemorySet();

int getByte( unsigned char* buf, int index );
int getWord( unsigned char* buf, int index );
int getDWord( unsigned char* buf, int index );

int main();
//boot_t readBootSector();

//void pbs();

#endif
