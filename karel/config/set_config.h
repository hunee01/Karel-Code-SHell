#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void set_ps1(const char *file , const char *key, const char *value, const char disp) {
    // RECONSTRUCT NEW JSON STRING
    FILE *conf = fopen(file, "w");
    if (!conf) {
        perror("open");
        return;
    }
    
    fprintf(conf, "{\n    \"disp\": %d,\n    \"ps1\": \"%s\"\n}\n",disp,value);
    fclose(conf);
}
