#include "pfe.h"
#include "fatSupport.h"
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>


void printMem()
{
//    int fildes;
    printf("%i\n", shrmem.fildes);
//    char *mode;
    printf("%s\n", shrmem.mode);
//    int bytesPerSector;
    printf("Bytes sector: %i\n", shrmem.bytesPerSector);
//    int sectorsPerCluster;
    printf("Bytes sector: %i\n", shrmem.sectorsPerCluster);
//    int numResSector;
    printf("Bytes sector: %i\n", shrmem.numResSector);
//    int numFat;
    printf("Bytes sector: %i\n", shrmem.numFat);
//    int maxRootDirEnt;
    printf("Bytes sector: %i\n", shrmem.maxRootDirEnt);
//    int totSectCount;
    printf("Bytes sector: %i\n", shrmem.totSectCount);
//    int sectPerFat;
    printf("Bytes sector: %i\n", shrmem.sectPerFat);
//    int sectPerTrack;
    printf("Bytes sector: %i\n", shrmem.sectPerTrack);
//    int numHead;
    printf("Bytes sector: %i\n", shrmem.numHead);
//    int totSectCount4Fat;
    printf("Bytes sector: %i\n", shrmem.totSectCount4Fat);
//    int bootSig;
    printf("Bytes sector: %i\n", shrmem.bootSig);
//    int volID;
    printf("Bytes sector: %i\n", shrmem.volID);
//    char volLabel[12];
    printf("Bytes sector: %s\n", shrmem.volLabel);
//    char fileSysTyp[9];
    printf("Bytes sector: %s\n", shrmem.fileSysTyp);
    return;
}


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


int main(int argc, char const **argv) {


    shl_shareMemoryGet();

    //printMem();

    long a,b;

    a = atoi(argv[1]);
    b = atoi(argv[2]);


    FILE_SYSTEM_ID = fopen("floppy2", "r");
    setFSID(FILE_SYSTEM_ID);
    setBPS(shrmem.bytesPerSector);


    if (FILE_SYSTEM_ID == NULL)
    {
        printf("Could not open the floppy drive or image.\n");
        exit(1);
    }


    unsigned char* buf;


    buf = readFAT12Table(1, 3);

    checkRange(a, b);

    for(; a <= b; a++){
        printf("%li: ", a);
        if (checkRange(a,b)){
            printf("%x\n", get_fat_entry(a, buf));
        }
        else
            break;
    }

    free(buf);

    fclose(FILE_SYSTEM_ID);

    return 0;
}

bool checkRange(int x, int y){
    bool retval = true;
    if (y < x){
        printf("Error, X < Y\n");
        retval = false;
    }
    if (x < 2){
        printf("Error, X < 2\n");
        retval = false;
    }
    return retval;
}

unsigned char* readFAT12Table(int x, int y){
    unsigned char* buf;
    buf = malloc(BYTES_PER_SECTOR);
    read_sector(0, buf);

    return buf;
}
