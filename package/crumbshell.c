#include "crumbshell.h"

int main(int argc, char const **argv) {
    bool run = true;

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
        shl_launcher(args);

        free(line);
        free(args);
    }
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
        if (execvp(args[0], args) == -1){
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

int shl_cd(char **args)
{
    if (args[1] == NULL) {
        printf("shl: expected argument to \"cd\"\n");
    } else {
        if (chdir(args[1]) != 0) {
            perror("shl");
        }
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
    return 0;
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
/*
bool input(){

    char *quit = "exit";
    char *pbs = "pbs";
    char *pfe = "pfe";
    char *clear = "clear";
    char *crumb = "crumbshell";
    int rc;
    char cmd[20];
    enum CMD command = NA;

    bool ret;
    printf("%s", crumbshell);

    scanf("%s", cmd);

    rc = strcmp(cmd, quit);
    if (rc == 0)
    {
        command = EXIT;
    }

    rc = strcmp(cmd, pbs);
    if (rc == 0)
    {
        command = PBS;
    }

    rc = strcmp(cmd, pfe);
    if (rc == 0)
    {
        command = PBS;
    }

    rc = strcmp(cmd, clear);
    if (rc == 0)
    {
        command = CLEAR;
    }

    rc = strcmp(cmd, crumb);
    if (rc == 0)
    {
        command = CRUMB;
    }

    ret = runcmd(command, cmd);

    return ret;
}
*/

//bool runcmd(enum CMD cmd, char* str){
//
//    bool ret = true;
//    pid_t pid;
//
//    switch (cmd) {
//        case EXIT:
//            printf("Good bye\n");
//            ret = false;
//            break;
//        case PBS:
//            pid=fork();
//            if (pid==0) { /* child process */
//                static char *argv[]={NULL};
//                execv("./pbs",argv);
//                exit(127); /* only if execv fails */
//            }
//            else { /* pid!=0; parent process */
//                waitpid(pid,0,0); /* wait for child to exit */
//            }
//        case PFE:
//            pid=fork();
//            if (pid==0) { /* child process */
//                static char *argv[]={NULL};
//                execv("./pfe",argv);
//                exit(127); /* only if execv fails */
//            }
//            else { /* pid!=0; parent process */
//                waitpid(pid,0,0); /* wait for child to exit */
//            }
//            break;
//        case CLEAR:
//            system( "clear");
//            break;
//        case CRUMB:
//            pid=fork();
//            if (pid==0) { /* child process */
//                static char *argv[]={NULL};
//                execv("./crumbshell",argv);
//                exit(127); /* only if execv fails */
//            }
//            else { /* pid!=0; parent process */
//                waitpid(pid,0,0); /* wait for child to exit */
//            }
//            break;
//        case NA:
//        default:
//            printf("[%s] is an invalid input\n", str);
//            break;
//    }
//    return ret;
//
//}
