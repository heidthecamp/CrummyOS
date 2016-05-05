#include "crumbshell.h"

int main(int argc, char const **argv) {

    shl_shareMemoryInit();

    shl_shareMemorySet(shrmem);

    //printMem();

    shl();



    return 0;
}


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


void shl(){
    char *line;
    char **args;
    while (1) {
        printf("%s", crumbshell);
        line = input();
        args = parse(line);
        shl_execute(args);

        free(line);
        free(args);

        shl_shareMemoryGet();

        /* FOR TESTING ONLY */
        //printMem(shrmem);
    }
}

void shl_shareMemoryInit(){
    int shm_ID = 0;

    if ((shm_ID = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666)) < 0){
        perror("Error creating SHM segment.\n");
        exit(EXIT_FAILURE);
    }

    //else
    //    printf("\nSHM segment has been created.\n");
}

void shl_shareMemorySet(){
    int shm_ID = 0;
    void *pointer = NULL;

    if ((shm_ID = shmget(SHMKEY, SHMSIZE, 0666)) < 0)
    {
        perror("Error getting SHM segment.");
        exit(-1);
    }
    //else
    //    printf("\nSHM segment has been found.\n");

    if ((pointer = shmat(shm_ID, NULL, 0)) == NULL){
        perror("Error including SHM address space.");
        exit(0);
    }
    //else
    //    printf("Allocating SHM to my address space.\n");
    printf("Copying message into shaged memory...\n");
    memcpy(pointer, &shrmem, sizeof(struct SharedMemory));

    /*
    if (shmdt(pointer) < 0){
        perror("Error deallocating shared memory.");
        exit(-1);
    }
    else
        printf("SHM has been deallocated.\n");
    */
}

void shl_shareMemoryGet(){
    int shm_ID = 0;
    void *pointer = NULL;
    if ((shm_ID = shmget(SHMKEY, SHMSIZE, 0444)) < 0 )
    {
        perror("Error getting SHM segment.");
        exit(-1);
    }
    //else
    //    printf("\nSHM segment has been found.\n");
    if ((pointer = shmat(shm_ID, NULL, 0)) == NULL)
    {
        perror("Error including SHM address space.");
        exit(0);
    }
    //else
    //    printf("Allocating SHM to my address space.\n");

    //printf("Fetching message from shared memory:\n");
    memcpy(&shrmem, pointer, sizeof(struct SharedMemory));




    /*
    if (shmdt(pointer) < 0)
    {
        perror("Error deallocating shared memory.");
        exit(-1);
    }
    else
        printf("SHM has been deallovated.\n");
    */
}


char *input(){
    char *line = NULL;
    size_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **parse(char *line){
    int bufsize = shl_tok_bufsize, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens){
        fprintf(stderr, "SHL: allocation error\n" );
        exit(EXIT_FAILURE);
    }

    token = strtok(line, SHL_TOK_DELIM);
    while (token != NULL){
        tokens[position] = token;
        position++;

        if (position >= bufsize) {
            bufsize += shl_tok_bufsize;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "SHL: allocation error\n" );
                exit(EXIT_FAILURE);
            }
        }
        token = strtok(NULL, SHL_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


int shl_launcher(char **args){
    pid_t pid, wpid;
    int status;

    pid = fork();
    if (pid == 0){
        //child
        if (execv(args[0], args) == -1){
            perror("shl");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error Forking
        perror("shl");
    } else {
        // Parrent
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    return 1;
}

int shl_help(char **args){
    int i;
    printf("Crumb Shell by Timothy Heidcamp\n");
    printf("Enter program names with arguments.\n");
    printf("The following are built in:\n");

    for (i = 0; i < shl_num_builtins(); i++) {
        printf("  %s\n", builtin_str[i]);
    }
    return 1;
}

int shl_exit(char **args){
    int shm_ID = 0;
    void *pointer = NULL;
    if ((shm_ID = shmget(SHMKEY, SHMSIZE, 0444)) < 0 )
    {
        perror("Error getting SHM segment.");
        exit(-1);
    }
    else
        printf("\nSHM segment has been found.\n");
    if ((pointer = shmat(shm_ID, NULL, 0)) == NULL)
    {
        perror("Error including SHM address space.");
        exit(0);
    }
    else
        printf("Allocating SHM to my address space.\n");


    if (shmdt(pointer) < 0){
        perror("Error deallocating shared memory.");
        exit(-1);
    }
    else
        printf("SHM has been deallocated.\n");

    exit(EXIT_SUCCESS);
}


int shl_execute(char **args) {
    int i;
    if (args[0] == NULL){
        //array was empty
        return 1;
    }

    for (i = 0; i < shl_num_builtins(); i++) {
        if (strcmp(args[0], builtin_str[i]) == 0) {
            return (*builtin_func[i])(args);
        }
    }

    return shl_launcher(args);
}
