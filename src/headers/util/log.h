#ifndef CJRPGENGINE_LOG_H
#define CJRPGENGINE_LOG_H

#include <string.h>

#define MAX_LOG_LINE_LENGTH 2048

const char *logLevels[] = {
        "error",
        "warn",
        "info",
        "debug"
};

typedef enum {
    ERROR = 0,
    WARN = 1,
    INFO = 2,
    DEBUG = 3,
} LogLevel;

LogLevel getLogLevelFromString(char *value) {
    int count = (sizeof(logLevels)) / sizeof(LogLevel);
    for (int i = 0; i < count - 1; i++) {
        if (strcmp(logLevels[i], value) == 0) {
            return i;
        }
    }
    return -1;
}

#include "src/util/log.c"

#endif //CJRPGENGINE_LOG_H
