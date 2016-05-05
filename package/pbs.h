#ifndef PBS_H
#define PBS_H


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

int main( int argc, char **argv );

#endif
