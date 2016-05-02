#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h> /* for fork */
#include <sys/types.h> /* for pid_t */
#include <sys/wait.h> /* for wait */

#ifndef CRUMBSHELL_H
#define CRUMBSHELL_H

#define SHL_TOK_DELIM " \t\r\n\a"



const int shl_in_bufsize = 1024;
const int shl_tok_bufsize = 64;


//bool input();
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
    "cd",
    "help",
    "exit"
};

int (*builtin_func[]) (char **) = {
    &shl_cd,
    &shl_help,
    &shl_exit
};

int shl_num_builtins() {
    return sizeof(builtin_str) / sizeof(char *);
}


#endif
