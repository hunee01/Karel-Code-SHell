#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *file = "kcsh.json";

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

ConfigInfo main_conf() {
    ConfigInfo result;
    
    FILE *conf = fopen(file,"r");
    fseek(conf, 0, SEEK_END);
    result.fileSize = ftell(conf);
    fseek(conf, 0, SEEK_SET);
    result.buffer = malloc(result.fileSize + 1);
    result.bytesRead = fread(result.buffer, 1, result.fileSize, conf);
    result.buffer[result.bytesRead] = '\0';
    fclose(conf);
    
    return result;
}

Initialisation init_conf(const char *buffer) {
    Initialisation result;
    
    // SETTING CONFIGS
    result.ps1 = strstr(buffer, "\"ps1\"") + 8;
    for (int i = 0; i < sizeof(result.ps1); i++) {
        if (result.ps1[i] == '"') {
            result.ps1[i] = '\0';
        }
    }
    
    result.configChar = strstr(buffer, "\"disp\"") + 8;
    result.disp = *result.configChar - '0';
    
    return result;
}

void set_ps1(const char *key, const char *value, const char disp) {
    // RECONSTRUCT NEW JSON STRING
    FILE *conf = fopen(file, "w");
    if (!conf) {
        perror("open");
        return;
    }
    
    fprintf(conf, "{\n    \"disp\": %d,\n    \"ps1\": \"%s\"\n}\n",disp,value);
    fclose(conf);
}

void set_disp(int setVal,char *buffer,const char *configChar,const long fileSize,size_t bytesRead) {
    FILE *conf = fopen(file,"r+");
    bytesRead = fread(buffer, 1, fileSize, conf);
    buffer[bytesRead] = '\0';
    
    configChar = strstr(buffer, "\"disp\"") + 8;
    char *configPtr = strchr(buffer,configChar[0]);
    fseek(conf,configPtr - buffer,SEEK_SET);
    if (fputc(setVal,conf) != EOF) {
        fclose(conf);
    }
    else {
        printf("conf: failed to set configuration: %c",setVal);
    }
}
