#ifndef BOOT_H
#define  BOOT_H
#include <stdio.h>
#include <stdlib.h>

#define BYTES_TO_READ_IN_BOOT_SECTOR 62


FILE* FILE_SYSTEM_ID;

typedef struct boot {
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
    char fileSysTyp[9];

} boot_t;

int getByte( unsigned char* buf, int index );
int getWord( unsigned char* buf, int index );
int getDWord( unsigned char* buf, int index );

boot_t readBootSector();

//void pbs();

#endif
