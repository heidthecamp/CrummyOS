/******************************************************************************
 * main: Sample for starting the FAT project.
 *
 * Authors:  Andy Kinley, Archana Chidanandan, David Mutchler and others.
 *           March, 2004.
 *****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "boot.h"
#include "fat.h"
#include "fatSupport.h"




bool checkRange( int x, int y){
    bool check = true;

    if (y < x)
    {
        printf("You messed up brah, y < x\n");
        check = false;
    }
    if (x < 2)
    {
        printf("You messed up brah, x < 2\n");
        check = false;
    }

    return check;
}


void pfe(){


    //char fat[];

    checkRange(3, 4);
    checkRange(4, 3); // y < x
    checkRange(1, 0); // y < x && x < 2
    checkRange(1, 3); // x < 2



}
