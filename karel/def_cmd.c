#include "src/command.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

int mode = 0777;

const char *varNames[3];

typedef struct {
    const char *USER;
    const char *VERSION;
    const char *HOME;
} EnvVars;
EnvVars envVars;

void command_init() {
    varNames[0] = "$VERSION";
    varNames[1] = "$USER";
    varNames[2] = "$HOME";
    
    envVars.USER = getenv("USER");
    envVars.VERSION = "v0.3.2-alpha";
    envVars.HOME = getenv("HOME");
}

void help() {
    // HELP: Prints list of commands
    
    printf("SDD kcsh, version %s",envVars.VERSION);
    printf("\nShell commands are defined internally. Type 'help' to see this list.");
    printf("\ncd [dir]");
    printf("\npwd");
    printf("\nrm [dir]");
    printf("\ntouch [dir]");
    printf("\nrd [dir]");
    printf("\nmkdir [dir]");
    printf("\nwhoami");
    printf("\necho [args]");
    printf("\nconf [args] [setting] [val]");
    printf("\nexit\n");
}

void echo(const char *args) {
    const char *text = args;
    // ECHO: Print the rest of the input
    if (strcmp(text,varNames[0]) == 0) {
        text = envVars.VERSION;
    }
    else if (strcmp(text,varNames[1]) == 0) {
        text = envVars.USER;
    }
    else if (strcmp(text,varNames[2]) == 0) {
        text = envVars.HOME;
    }
    else if (strcmp(text,"") == 0) {
        text = "echo";
    }
    else {
        text = args;
    }
    
    printf("%s\n",text);
}

void pwd(const char *cwd) {
    // PWD: Print working directory
    printf("%s\n",cwd);
}

void cd(const char *dir) {
    const char *target;
    if (dir == NULL || dir[0] == '\0') { // GO TO HOME IF EMPTY AND HOME NOT NULL
        if (envVars.HOME == NULL || envVars.HOME[0] == '\0') {
            printf("cd: HOME not set\n");
            return;
        }
        target = envVars.HOME;
    }
    else {
        target = dir;
    }

    if (chdir(target) != 0) {
        perror("cd");
    }
}

void rm(const char *file) {
    // RM: Remove file
    if (remove(file) != 0) {
        perror("rm");
    }
    else {
        remove(file);
    }
}

void c_mkdir(const char *dir) {
    // MKDIR: Make directory
    if (mkdir(dir,mode) != 0) {
        perror("mkdir");
    }
    else {
        mkdir(dir,mode);
    }
}

void touch(const char *file) {
    // TOUCH: Make file
    FILE *ftmk = fopen(file, "w");
    
    if (ftmk != 0) {
        fclose(ftmk);
    }
    else {
        perror("touch");
    }
}

void rd(const char *file) {
    // RD: Read and display
    FILE *ftpr = fopen(file, "r");
    
    char ch;
    if (ftpr != 0) {
        while ((ch = fgetc(ftpr)) != EOF) {
            printf("%c",ch);
        }
        fclose(ftpr);
    }
    else {
        perror("rd");
    }
}

void whoami() {
    // WHOAMI: Print current user
    printf("%s\n", envVars.USER);
}
