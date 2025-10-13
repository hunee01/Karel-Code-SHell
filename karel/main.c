#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <limits.h>
#include <sys/stat.h>

/* -- Sleep Deprived Dev: Karel-Code SHell --
* 
* Originally made in CodeHS, hence the name,
* and an hour of C experience.
*/

int main()
{
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
    char *VERSION = "0.1-beta";
    gethostname(hostname, sizeof(hostname));
    char *USER = getenv("USER");
    int disp = 0;
    int mode = 0777;
    
    // MAIN LOOP
    while (1)
    {
        getcwd(cwd, sizeof(cwd));
        
        // GET USER INPUT
        switch (disp)
        {
            case 0:
                printf("\n& ");
                break;
            case 1:
                if (cwd != NULL)
                {
                    printf("\n%s@%s:%s & ",USER,hostname,cwd);
                }
                else 
                {
                    printf("\n%s@%s:~ & ",USER,hostname);
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
        while (*args == ' ')
        {
            args++;
        }

        // CMD PROCESSING
        if (strcmp(cmd, "help") == 0)
        {
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
            printf("\ndisp");
            printf("\nexit");
        }
        else if (strcmp(cmd, "echo") == 0)
        {
            // ECHO: Print the rest of the input
            printf("%s", args);
        }
        else if (strcmp(cmd, "pwd") == 0)
        {
            // PWD: Print working directory
            printf("%s", cwd);
        }
        else if (strcmp(cmd, "cd") == 0)
        {
            // CD: Change directory
            if (chdir(args) != 0)
            {
                printf("cd: no such file or directory: %s",args);
            }
            else
            {
                chdir(args);
            }
        }
        else if (strcmp(cmd, "rm") == 0)
        {
            // RM: Remove file
            if (remove(args) != 0)
            {
                printf("rm: no such file or directory: %s",args);
            }
            else
            {
                remove(args);
            }
        }
        else if (strcmp(cmd, "mkdir") == 0)
        {
            // MKDIR: Make directory
            if (mkdir(args,mode) != 0)
            {
                printf("mkdir: no such file or directory: %s",args);
            }
            else
            {
                mkdir(args,mode);
            }
        }
        else if (strcmp(cmd, "touch") == 0)
        {
            // TOUCH: Make file
            FILE *ftmk = fopen(args, "w");
            
            if (ftmk != 0)
            {
                fclose(ftmk);
            }
            else
            {
                printf("touch: no such file or directory: %s",args);
            }
        }
        else if (strcmp(cmd, "rd") == 0)
        {
            // RD: Read and display
            FILE *ftpr = fopen(args, "r");
            
            char ch;
            if (ftpr != 0)
            {
                while ((ch = fgetc(ftpr)) != EOF)
                {
                    printf("%c",ch);
                }
                fclose(ftpr);
            }
            else
            {
                printf("rd: no such file or directory: %s",args);
            }
        }
        else if (strcmp(cmd, "whoami") == 0)
        {
            // WHOAMI: Print current user
            printf("%s", getenv("USER"));
        }
        else if (strcmp(cmd, "disp") == 0)
        {
            // DISP: Display information at line header
            switch (disp)
            {
                case 0:
                    disp = 1;
                    break;
                case 1:
                    disp = 0;
                    break;
            }
        }
        else if (strcmp(cmd, "exit") == 0)
        {
            // EXIT: Exit the loop
            break;
        }
        else
        {
            // FALLBACK: Print unknown command
            printf("kcsh: %s: command not found",cmd);
        }
    }

    return 0;
}
