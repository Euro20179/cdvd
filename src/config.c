#include "config.h"

#include <stdlib.h>
#include <string.h>

const char* getConfigPath(char* dest){
    char* configSuffix = "/cdvd/";
    strcat(dest, getenv("XDG_CONFIG_HOME"));
    if(dest[0] == '\0'){
        strcat(dest, getenv("HOME"));
    }
    strcat(dest, configSuffix);
    return dest;
}

void globalConfigPath(char* dest){
    strcpy(dest, "/etc/cdvd/");
}
