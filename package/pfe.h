#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef PFE_H
#define PFE_H

FILE * FILE_SYSTEM_ID;

#define SHMKEY  2113
#define SHMSIZE 500


typedef struct SharedMemory{
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
}SHR_MEM;

struct SharedMemory shrmem;

void shl_shareMemoryGet();
void shl_shareMemorySet();

void pfe(int x, int y);

bool checkRange(int x, int y);

unsigned char* readFAT12Table(int x, int y);

#endif
