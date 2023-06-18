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
        time_t t = time(NULL);
        struct tm *tm = localtime(&t);
        char s[64];
        strftime(s, sizeof(s), "%Y-%m-%d %H:%M:%S", tm);
        char toLog[1024];
        sprintf(toLog, "[%s - %s] %s\n", s, getLogLevelString(logLevel), message);
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
