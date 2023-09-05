#ifndef CJRPGENGINE_LOG_H
#define CJRPGENGINE_LOG_H

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

#endif //CJRPGENGINE_LOG_H
