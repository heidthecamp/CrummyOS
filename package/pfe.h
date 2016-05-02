#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef PFE_H
#define PFE_H

void pfe(int x, int y);

bool checkRange(int x, int y);

unsigned char* readFAT12Table(int x, int y);

#endif
