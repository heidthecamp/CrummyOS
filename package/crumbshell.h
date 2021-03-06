#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHMKEY  2113
#define SHMSIZE 500



#ifndef CRUMBSHELL_H
#define CRUMBSHELL_H

#define SHL_TOK_DELIM " \t\r\n\a"

FILE * FILE_SYSTEM_ID;

typedef struct SharedMemory{
    int fildes;
    char mode[3];
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

const int shl_in_bufsize = 1024;
const int shl_tok_bufsize = 64;


void shl();
void shl_shareMemoryInit();
void shl_shareMemoryGet();
void shl_shareMemorySet();
void printMem();
char *input();
char **parse(char *line);
int shl_launcher(char **);
int shl_cd(char **args);
int shl_help(char **args);
int shl_exit(char **args);

//bool runcmd();
void shl();

char crumbshell[13] = "CrumbShell > ";


char *builtin_str[] = {
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &shl_help,
    &shl_exit
};

int shl_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}

int shl_execute(char **args);


#endif
