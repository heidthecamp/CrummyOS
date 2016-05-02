#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "boot.h"
#include "fatSupport.h"


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


boot_t readBootSector(){
    unsigned char* buf;
    boot_t boot;
    buf = malloc(BYTES_TO_READ_IN_BOOT_SECTOR);

    setBPS(BYTES_TO_READ_IN_BOOT_SECTOR);


    if (read_sector(0, buf) == -1){
        printf("Something has gone wrong -- could not read the boot sector\n");
    }

    boot.bytesPerSector = getWord(buf, 11);
    setBPS(boot.bytesPerSector);
    boot.sectorsPerCluster = getByte(buf, 13);
    boot.numResSector = getWord(buf, 14 );
    boot.numFat = getByte(buf, 16);
    boot.maxRootDirEnt = getWord(buf, 17);
    boot.totSectCount = getWord(buf, 19);
    boot.sectPerFat = getWord(buf, 22);
    boot.sectPerTrack = getWord(buf, 24);
    boot.numHead = getWord(buf, 26);
    boot.totSectCount4Fat = getDWord(buf, 32);
    boot.bootSig = getByte(buf, 38);
    boot.volID = getDWord(buf, 39);


    boot.volLabel[11] = 0;
    boot.fileSysTyp[8] = 0;
    memcpy(boot.volLabel, buf + 43, 11);
    memcpy(boot.fileSysTyp, buf + 54, 8);

    free(buf);

    return boot;
}
