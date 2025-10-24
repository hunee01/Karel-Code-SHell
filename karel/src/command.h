#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int mode = 0777;

const char *varNames[2];

typedef struct {
    char *USER;
    char *VERSION;
} EnvVars;
EnvVars envVars;

void command_init() {
    varNames[0] = "$VERSION";
    varNames[1] = "$USER";
    
    envVars.USER = getenv("USER");
    envVars.VERSION = "v0.3-beta";
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
    printf("\nexit");
}

void echo(char *text) {
    // ECHO: Print the rest of the input
    if (strcmp(text,varNames[0]) == 0) {
        text = envVars.VERSION;
    }
    else if (strcmp(text,varNames[1]) == 0) {
        text = envVars.USER;
    }
    
    printf("%s",text);
}

void pwd(const char *cwd) {
    // PWD: Print working directory
    printf("%s",cwd);
}

void cd(const char *dir) {
    // CD: Change directory
    if (chdir(dir) != 0) {
        printf("cd: no such file or directory: %s",dir);
    }
    else {
        chdir(dir);
    }
}

void rm(const char *file) {
    // RM: Remove file
    if (remove(file) != 0) {
        printf("rm: no such file or directory: %s",file);
    }
    else {
        remove(file);
    }
}

void c_mkdir(const char *dir) {
    // MKDIR: Make directory
    if (mkdir(dir,mode) != 0) {
        printf("mkdir: no such file or directory: %s",dir);
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
        printf("touch: no such file or directory: %s",file);
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
        printf("rd: no such file or directory: %s",file);
    }
}

void whoami() {
    // WHOAMI: Print current user
    printf("%s", getenv("USER"));
}
