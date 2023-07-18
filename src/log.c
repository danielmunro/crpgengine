typedef struct {
    LogLevel level;
} Log;

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

Log *createLog(LogLevel configuredLogLevel) {
    Log *log = malloc(sizeof(Log));
    log->level = configuredLogLevel;
    return log;
}

void addLogWithLevel(Log *log, LogLevel logLevel, char *message, va_list ag) {
    if (log->level >= logLevel) {
        char toLog[MAX_LOG_LINE_LENGTH];
        sprintf(toLog, "%s -- %s\n", getLogLevelString(logLevel), message);
        vprintf(toLog, ag);
    }
}

void addDebug(Log *log, char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(log, DEBUG, message, args);
    va_end(args);
}

void addInfo(Log *log, char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(log, INFO, message, args);
    va_end(args);
}

void addWarning(Log *log, char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(log, WARN, message, args);
    va_end(args);
}

void addError(Log *log, char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(log, ERROR, message, args);
    va_end(args);
}

void addFatal(Log *log, char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(log, ERROR, message, args);
    va_end(args);
    exit(EXIT_MISSING_SPRITESHEET);
}
