#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

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
    
    #include "config/set_config.h" // CALL HEADER
    
    char input[1024];
    char cmd[1024];
    char cwd[1024];
    int pos;
    char hostname[1024];
    char *VERSION = "0.2.4-beta.1";
    gethostname(hostname, sizeof(hostname));
    char *USER = getenv("USER");
    int mode = 0777;
    char *config = "config/kcsh.json";
    
    //int disp = 0;

    // MAIN LOOP
    while (1) {
        // READ CONFIGS & DETERMINING FILE SIZE
        FILE *configFile = fopen(config,"r");
        fseek(configFile, 0, SEEK_END);
        long fileSize = ftell(configFile);
        fseek(configFile, 0, SEEK_SET);
        char *buffer = malloc(fileSize + 1);
        size_t bytesRead = fread(buffer, 1, fileSize, configFile);
        buffer[bytesRead] = '\0';
        fclose(configFile);
        
        // SETTING CONFIGS
        char *ps1 = strstr(buffer, "\"ps1\"") + 8;
        for (int i = 0; i < sizeof(ps1); i++) {
            if (ps1[i] == '"') {
                ps1[i] = '\0';
            }
        }
        
        char *configChar = strstr(buffer, "\"disp\"") + 8;
        int disp = *configChar - '0';
        
        getcwd(cwd, sizeof(cwd));
        
        // GET USER INPUT
        switch (disp) {
            case 0:
                printf("\n%s ",ps1);
                break;
            case 1:
                if (cwd != NULL) {
                    printf("\n%s@%s:%s %s ",USER,hostname,cwd,ps1);
                }
                else {
                    printf("\n%s@%s:~ %s ",USER,hostname,ps1);
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
            printf("SDD kcsh, version %s",VERSION);
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
        else if (strcmp(cmd, "echo") == 0) {
            // ECHO: Print the rest of the input
            printf("%s", args);
        }
        else if (strcmp(cmd, "pwd") == 0) {
            // PWD: Print working directory
            printf("%s", cwd);
        }
        else if (strcmp(cmd, "cd") == 0) {
            // CD: Change directory
            if (chdir(args) != 0) {
                printf("cd: no such file or directory: %s",args);
            }
            else {
                chdir(args);
            }
        }
        else if (strcmp(cmd, "rm") == 0) {
            // RM: Remove file
            if (remove(args) != 0) {
                printf("rm: no such file or directory: %s",args);
            }
            else {
                remove(args);
            }
        }
        else if (strcmp(cmd, "mkdir") == 0) {
            // MKDIR: Make directory
            if (mkdir(args,mode) != 0) {
                printf("mkdir: no such file or directory: %s",args);
            }
            else {
                mkdir(args,mode);
            }
        }
        else if (strcmp(cmd, "touch") == 0) {
            // TOUCH: Make file
            FILE *ftmk = fopen(args, "w");
            
            if (ftmk != 0) {
                fclose(ftmk);
            }
            else {
                printf("touch: no such file or directory: %s",args);
            }
        }
        else if (strcmp(cmd, "rd") == 0) {
            // RD: Read and display
            FILE *ftpr = fopen(args, "r");
            
            char ch;
            if (ftpr != 0) {
                while ((ch = fgetc(ftpr)) != EOF) {
                    printf("%c",ch);
                }
                fclose(ftpr);
            }
            else {
                printf("rd: no such file or directory: %s",args);
            }
        }
        else if (strcmp(cmd, "whoami") == 0) {
            // WHOAMI: Print current user
            printf("%s", getenv("USER"));
        }
        else if (strcmp(cmd, "conf") == 0) {
            // CONF: Edit configurations
            char setting[64] = {0};
            for (int i = 0; i < strlen(args); i++) {
                if (args[i] == ' ') {
                    strncpy(setting, args + i + 1, sizeof(setting) - 1);
                    setting[sizeof(setting) - 1] = '\0';
                    args[i] = '\0';
                }
            }
            if (strcmp(args,"set") == 0) {
                char setVal[64] = {0};
                for (int i = 0; i < strlen(setting); i++) {
                    if (setting[i] == ' ') {
                        strncpy(setVal, setting + i + 1, sizeof(setVal) - 1);
                        setVal[sizeof(setVal) - 1] = '\0';
                        setting[i] = '\0';
                    }
                }
                
                // LOOK FOR SETTINGS
                if (strcmp(setting,"disp") == 0) {
                    if (*setVal == '0' || *setVal == '1') {
                        configFile = fopen(config,"r+");
                        bytesRead = fread(buffer, 1, fileSize, configFile);
                        buffer[bytesRead] = '\0';
                        
                        configChar = strstr(buffer, "\"disp\"") + 8;
                        char *configPtr = strchr(buffer,configChar[0]);
                        fseek(configFile,configPtr - buffer,SEEK_SET);
                        if (fputc(*setVal,configFile) != EOF) {
                            fclose(configFile);
                        }
                        else {
                            printf("conf: failed to set configuration: %c",*setVal);
                        }
                    }
                    else {
                        printf("conf: value for 'disp' must be 0 or 1: %c",*setVal);
                    }
                }
                else if (strcmp(setting,"ps1") == 0) {
                    if (strlen(setVal) < 8) {
                        set_ps1(config, "ps1", setVal, disp);
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
                    printf("%d",disp);
                }
                else if (strcmp(setting,"ps1") == 0) {
                    printf("%s",ps1);
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
