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
        printf("Snarl... Incompatible OS\n");
    #elif _WIN64
        printf("Snarl... Incompatible OS\n");
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
    
    // MAIN LOOP
    while (1) {
        // READ CONFIGS & DETERMINING FILE SIZE

        //printf("[DEBUG] Entering loop...\n");
        ConfigInfo confInfo = main_conf();
        //printf("[DEBUG] main_conf() OK\n");
        Initialisation configs = init_conf(confInfo.buffer);
        //printf("[DEBUG] init_conf() OK\n");
        command_init(); // INITIALISE COMMANDS
        //printf("[DEBUG] command_init() OK\n");
        
        getcwd(cwd, sizeof(cwd));
        
        // GET USER INPUT
        switch (configs.disp) {
            case 0:
                printf("%s ",configs.ps1);
                break;
            case 1:
                if (cwd[0] != '\0') {
                    printf("%s@%s:%s %s ",getenv("USER"),hostname,cwd,configs.ps1);
                }
                else {
                    printf("%s@%s:~ %s ",getenv("USER"),hostname,configs.ps1);
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
        char *args = NULL;
        if (input[i] != '\0') {
            args = input + i;
            while (*args == ' ') {
                args++;
            }
            if (*args == '\0') {
                args = NULL; // NO ARGS
            }
        }
        
        if (args == NULL || args[0] == '\0') {
            args = "";
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
        // CD: Changes directory
            cd(args);
        }
        else if (strcmp(cmd, "rm") == 0) {
            // RM: Remove file
            char ftdel[64] = {0};
            for (int i = 0; i < (int)strlen(args); i++) {
                if (args[i] == ' ') {
                    strncpy(ftdel, args + i + 1, sizeof(ftdel) - 1);
                    ftdel[sizeof(ftdel) - 1] = '\0';
                    args[i] = '\0';
                }
            }
            if (strcmp(args,"-r") == 0 || strcmp(args,"--recursive") == 0) {
                rm("-r",ftdel);
            }
            else {
                rm(NULL,args);
            }
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
            if (strcmp(args,"-s") == 0 || strcmp(args,"--set") == 0) {
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
                        printf("conf: value for 'disp' must be 0 or 1: %c\n",*setVal);
                    }
                }
                else if (strcmp(setting,"ps1") == 0) {
                    if (strlen(setVal) < 8) {
                        set_ps1(setVal, configs.disp);
                    }
                    else {
                        printf("conf: value for 'ps1' must be less than 8 characters: %s\n",setVal);
                    }
                }
                else {
                    printf("conf: setting does not exist: %s\n",setting);
                }
            }
            else if (strcmp(args,"-p") == 0 || strcmp(args,"--print") == 0) {
                if (strcmp(setting,"disp") == 0) {
                    printf("%d\n",configs.disp);
                }
                else if (strcmp(setting,"ps1") == 0) {
                    printf("%s\n",configs.ps1);
                }
                else
                {
                    printf("conf: invalid argument: %s\n",setting);
                }
            }
            else {
                printf("conf: invalid argument: %s\n",args);
            }
        }
        else if (strcmp(cmd, "exit") == 0) {
            // EXIT: Exit the loop
            break;
        }
        else if (strcmp(cmd, "") == 0) {
            printf("");
        }
        else {
            // FALLBACK: Print unknown command
            printf("kcsh: %s: command not found\n",cmd);
        }

        free(confInfo.buffer);
        free(configs.ps1);
    }
    return 0;
}
