#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

#include "config/set_config.h" // CALL CONFIG HEADER
#include "src/command.h" // CALL COMMAND HEADER

int main() {
    #ifdef _WIN32
        printf("WARNING: Incompatible OS. This program is not compatible with Windows 32-bit.\n");
    #elif _WIN64
        printf("WARNING: Incompatible OS. This program is not compatible with Windows 64-bit.\n");
    #elif __linux__
        printf("Bark!\n");
    #elif __APPLE__
        printf("Bark!\n");
    #endif
    
    char input[1024];
    char cmd[1024];
    char cwd[1024];
    int pos;
    char hostname[1024];
    
    gethostname(hostname, sizeof(hostname));
    //char *config = "config/kcsh.json";       <-- KEEP ME UFN!
    
    // MAIN LOOP
    while (1) {
        // READ CONFIGS & DETERMINING FILE SIZE
        ConfigInfo confInfo = main_conf();
        Initialisation configs = init_conf(confInfo.buffer);
        command_init(); // INITIALISE COMMANDS
        
        getcwd(cwd, sizeof(cwd));
        
        // GET USER INPUT
        switch (configs.disp) {
            case 0:
                printf("\n%s ",configs.ps1);
                break;
            case 1:
                if (cwd[0] != '\0') {
                    printf("\n%s@%s:%s %s ",getenv("USER"),hostname,cwd,configs.ps1);
                }
                else {
                    printf("\n%s@%s:~ %s ",getenv("USER"),hostname,configs.ps1);
                }
                break;
        }
        
        fgets(input, sizeof(input), stdin);

        // REMOVE NEWLINE CHARACTER IF PRESENT
        input[strcspn(input, "\n")] = '\0';

        // RESET cmd and pos
        memset(cmd, 0, sizeof(cmd));
        pos = 0;

        // SET CMD INDEX TO ALL INPUT INDEX BEFORE SPACE OR NULL
        int i = 0;
        while (input[i] != ' ' && input[i] != '\0') {
            cmd[i] = input[i];
            i++;
        }
        cmd[i] = '\0';
        pos = i + 1;
        char *args = input + pos;
        while (*args == ' ') {
            args++;
        }

        // CMD PROCESSING
        if (strcmp(cmd, "help") == 0) {
            // HELP: Print list of commands
            help();
        }
        else if (strcmp(cmd, "echo") == 0) {
            // ECHO: Print the rest of the input
            echo(args);
        }
        else if (strcmp(cmd, "pwd") == 0) {
            // PWD: Print working directory
            pwd(cwd);
        }
        else if (strcmp(cmd, "cd") == 0) {
            // CD: Change directory
            cd(args);
        }
        else if (strcmp(cmd, "rm") == 0) {
            // RM: Remove file
            rm(args);
        }
        else if (strcmp(cmd, "mkdir") == 0) {
            // MKDIR: Make directory
            c_mkdir(args);
        }
        else if (strcmp(cmd, "touch") == 0) {
            // TOUCH: Make file
            touch(args);
        }
        else if (strcmp(cmd, "rd") == 0) {
            // RD: Read and display
            rd(args);
        }
        else if (strcmp(cmd, "whoami") == 0) {
            // WHOAMI: Print current user
            whoami();
        }
        else if (strcmp(cmd, "conf") == 0) {
            // CONF: Edit configurations
            char setting[64] = {0};
            for (int i = 0; i < (int)strlen(args); i++) {
                if (args[i] == ' ') {
                    strncpy(setting, args + i + 1, sizeof(setting) - 1);
                    setting[sizeof(setting) - 1] = '\0';
                    args[i] = '\0';
                }
            }
            if (strcmp(args,"set") == 0) {
                char setVal[64] = {0};
                for (int i = 0; i < (int)strlen(setting); i++) {
                    if (setting[i] == ' ') {
                        strncpy(setVal, setting + i + 1, sizeof(setVal) - 1);
                        setVal[sizeof(setVal) - 1] = '\0';
                        setting[i] = '\0';
                    }
                }
                
                // LOOK FOR SETTINGS
                if (strcmp(setting,"disp") == 0) {
                    if (*setVal == '0' || *setVal == '1') {
                        set_disp(*setVal,confInfo.buffer,configs.configChar,confInfo.fileSize,confInfo.bytesRead);
                    }
                    else {
                        printf("conf: value for 'disp' must be 0 or 1: %c",*setVal);
                    }
                }
                else if (strcmp(setting,"ps1") == 0) {
                    if (strlen(setVal) < 8) {
                        set_ps1(setVal, configs.disp);
                    }
                    else {
                        printf("conf: value for 'ps1' must be less than 8 characters: %s",setVal);
                    }
                }
                else {
                    printf("conf: setting does not exist: %s",setting);
                }
            }
            else if (strcmp(args,"prt") == 0) {
                if (strcmp(setting,"disp") == 0) {
                    printf("%d",configs.disp);
                }
                else if (strcmp(setting,"ps1") == 0) {
                    printf("%s",configs.ps1);
                }
                else
                {
                    printf("conf: invalid argument: %s",setting);
                }
            }
            else {
                printf("conf: invalid argument: %s",args);
            }
        }
        else if (strcmp(cmd, "exit") == 0) {
            // EXIT: Exit the loop
            break;
        }
        else {
            // FALLBACK: Print unknown command
            printf("kcsh: %s: command not found",cmd);
        }
    }
    return 0;
}
