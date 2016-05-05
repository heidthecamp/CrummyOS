#include "crumbshell.h"

int main(int argc, char const **argv) {
    bool run = true;

    shl_shareMemoryInit();

    shl_shareMemorySet("Test");

    shl_shareMemoryGet();

    shl();

    return 0;
}

void shl(){
    char *line;
    char **args;
    bool run = true;
    while (run) {
        printf("%s", crumbshell);
        line = input();
        args = parse(line);
        shl_execute(args);

        free(line);
        free(args);
    }
}

void shl_shareMemoryInit(){
    int shm_ID = 0;
    void *pointer = NULL;

    if ((shm_ID = shmget(SHMKEY, SHMSIZE, IPC_CREAT | 0666)) < 0){
        perror("Error creating SHM segment.\n");
        exit(EXIT_FAILURE);
    }

    else
        printf("\nSHM segment has been created.\n");
}

void shl_shareMemorySet(char * toShare){
    int shm_ID = 0;
    void *pointer = NULL;

    if ((shm_ID = shmget(SHMKEY, SHMSIZE, 0666)) < 0)
    {
        perror("Error getting SHM segment.");
        exit(-1);
    }
    else
        printf("\nSHM segment has been found.\n");

    if ((pointer = shmat(shm_ID, NULL, 0)) == NULL){
        perror("Error including SHM address space.");
        exit(0);
    }
    else
        printf("Allocating SHM to my address space.\n");
    printf("Copying message into shaged memory...\n");
    memcpy(pointer, toShare, strlen(toShare) + 1);

    if (shmdt(pointer) < 0){
        perror("Error deallocating shared memory.");
        exit(-1);
    }
    else
        printf("SHM has been deallocated.\n");
}

void shl_shareMemoryGet(){
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

    printf("Fetching message from shared memory:\n");
    sprintf("\n%s\n", (char*) pointer);
    if (shmdt(pointer) < 0)
    {
        perror("Error deallocating shared memory.");
        exit(-1);
    }
    else
        printf("SHM has been deallovated.\n");

}


char *input(){
    char *line = NULL;
    ssize_t bufsize = 0;
    getline(&line, &bufsize, stdin);
    return line;
}

char **parse(char *line){
    int bufsize = shl_tok_bufsize, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;

    if (!tokens){
        printf(stderr, "SHL: allocation error\n" );
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
                printf(stderr, "SHL: allocation error\n" );
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
