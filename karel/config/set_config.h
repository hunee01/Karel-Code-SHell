#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <errno.h>

//#define DEFAULT_RELATIVE_PATH "config/kcsh.json"
char *file = "config/kcsh.json";

typedef struct {
    char *configChar;
    int disp;
    char *ps1;
} Initialisation;

typedef struct {
    long fileSize;
    char *buffer;
    size_t bytesRead;
} ConfigInfo;

/*
char *open_config_file() {
    char *home = getenv("HOME");
    char *path = strcat("/",file);
    return strcat(home,path);
}
*/

ConfigInfo main_conf() {
    //char *teststr = open_config_file();
    //printf("%s",teststr);
    ConfigInfo result;
    result.fileSize = 0;
    result.buffer = NULL;
    result.bytesRead = 0;

    FILE *conf = fopen(file,"r"); // FAILED TO OPEN NECESSARY CONFIG FILES
    if (!conf) {
        // CONTINUE SAFE MODE
        perror("c: main_conf");
        result.buffer = malloc(1);
        if (result.buffer) result.buffer[0] = '\0';
        return result;
    }

    if (fseek(conf, 0, SEEK_END) != 0) {
        perror("c: fseek");
        fclose(conf);
        result.buffer = malloc(1);
        if (result.buffer) result.buffer[0] = '\0';
        return result;
    }

    long sz = ftell(conf);
    if (sz < 0) {
        perror("c: ftell");
        fclose(conf);
        result.buffer = malloc(1);
        if (result.buffer) result.buffer[0] = '\0';
        return result;
    }
    result.fileSize = sz;

    if (fseek(conf, 0, SEEK_SET) != 0) {
        perror("c: fseek");
        fclose(conf);
        result.buffer = malloc(1);
        if (result.buffer) result.buffer[0] = '\0';
        return result;
    }

    result.buffer = malloc(result.fileSize + 1);
    if (!result.buffer) {
        perror("c: malloc");
        fclose(conf);
        result.fileSize = 0;
        result.bytesRead = 0;
        return result;
    }

    result.bytesRead = fread(result.buffer, 1, result.fileSize, conf);
    result.buffer[result.bytesRead] = '\0';
    fclose(conf);

    return result;
}

Initialisation init_conf(const char *buffer) {
    Initialisation result = {0};

    if (!buffer) { // DEFAULTS IF NO BUFFER
        result.disp = 0;
        result.ps1 = strdup("&");
        return result;
    }

    // FIND PS1
    char *bufferPS1 = strstr(buffer, "\"ps1\"");
    if (bufferPS1) {
        bufferPS1 = strchr(bufferPS1, ':');
        if (bufferPS1) {
            bufferPS1++;
            while (*bufferPS1 == ' ' || *bufferPS1 == '\"')
                bufferPS1++;

            char *bufferPS1End = strchr(bufferPS1, '\"');
            if (bufferPS1End) {
                size_t len = bufferPS1End - bufferPS1;
                result.ps1 = malloc(len + 1);
                if (result.ps1) {
                    strncpy(result.ps1, bufferPS1, len);
                    result.ps1[len] = '\0';
                }
            }
        }
    }

    // FIND DISP
    char *bufferDisp = strstr(buffer, "\"disp\"");
    if (bufferDisp) {
        bufferDisp = strchr(bufferDisp, ':');
        if (bufferDisp) {
            bufferDisp++;
            while (*bufferDisp == ' ' || *bufferDisp == '\"')
                bufferDisp++;
            result.disp = *bufferDisp - '0';
        }
    }

    if (!result.ps1) { // PS1 FALLBACK
        result.ps1 = strdup("&");
    }

    result.configChar = NULL;
    return result;
}

void set_ps1(const char *value, const char disp) {
    // RECONSTRUCT NEW JSON STRING
    FILE *conf = fopen(file, "w");
    if (!conf) {
        perror("open");
        return;
    }

    fprintf(conf, "{\n    \"disp\": %d,\n    \"ps1\": \"%s\"\n}\n",disp,value);
    fclose(conf);
}

void set_disp(int setVal,char *buffer,char *configChar,const long fileSize,size_t bytesRead) {
    FILE *conf = fopen(file,"r+");
    if (!conf) {
        perror("open");
        return;
    }
    
    bytesRead = fread(buffer, 1, fileSize, conf);
    buffer[bytesRead] = '\0';
    
    char *littleBuffer = strstr(buffer,"\"disp\"");
    int charPtr = 0;
    for (int i = 0; i < (int)strlen(littleBuffer) - 1; i++) {
        if (littleBuffer[i + 7] != ' ' || littleBuffer[i + 7] != '\n') {
            charPtr = (i + 8) + ((int)strlen(buffer) - (int)strlen(littleBuffer));
            break;
        }
    }
    
    configChar = buffer + charPtr;
    char *configPtr = strchr(buffer,configChar[0]);
    fseek(conf,configPtr - buffer,SEEK_SET);
    if (fputc(setVal,conf) != EOF) {
        fclose(conf);
    }
    else {
        printf("conf: failed to set configuration: %c",setVal);
    }
}
