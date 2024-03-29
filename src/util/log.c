#include <mm_malloc.h>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>

typedef enum {
    ERROR = 0,
    WARN = 1,
    INFO = 2,
    DEBUG = 3,
} LogLevel;

typedef struct {
    LogLevel level;
} Log;

Log *logger;

const char *logLevels[] = {
        "error",
        "warn",
        "info",
        "debug"
};

LogLevel getLogLevelFromString(const char *value) {
    int count = (sizeof(logLevels)) / sizeof(LogLevel);
    for (int i = 0; i < count - 1; i++) {
        if (strcmp(logLevels[i], value) == 0) {
            return i;
        }
    }
    return -1;
}

char *getLogLevelString(LogLevel logLevel) {
    if (logLevel == DEBUG) {
        return "DEBUG";
    } else if (logLevel == INFO) {
        return "INFO";
    } else if (logLevel == WARN) {
        return "WARN";
    } else if (logLevel == ERROR) {
        return "ERROR";
    } else {
        return "UNKNOWN";
    }
}

void addLogWithLevel(LogLevel logLevel, const char *message, va_list ag) {
    if (logger->level >= logLevel) {
        char toLog[MAX_LOG_LINE_LENGTH];
        sprintf(toLog, "%s -- %s\n", getLogLevelString(logLevel), message);
        vprintf(toLog, ag);
    }
}

void addDebug(const char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(DEBUG, message, args);
    va_end(args);
}

void addInfo(const char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(INFO, message, args);
    va_end(args);
}

void addWarning(const char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(WARN, message, args);
    va_end(args);
}

void addError(const char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(ERROR, message, args);
    va_end(args);
}

Log *createLog(LogLevel configuredLogLevel) {
    logger = malloc(sizeof(Log));
    logger->level = configuredLogLevel;
    addInfo("log level set to %s", getLogLevelString(logger->level));
    return logger;
}
