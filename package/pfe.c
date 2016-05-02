#include "pfe.h"
#include "fatSupport.h"
#include "boot.h"

int main(int argc, char const **argv) {
    boot_t boot;


    long a,b;

    a = atoi(argv[1]);
    b = atoi(argv[2]);

    FILE_SYSTEM_ID = fopen("floppy1", "r+");
    setFSID(FILE_SYSTEM_ID);

    if (FILE_SYSTEM_ID == NULL)
    {
        printf("Could not open the floppy drive or image.\n");
        exit(1);
    }

    boot = readBootSector();


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
    read_sector(x, buf);

    return buf;
}
