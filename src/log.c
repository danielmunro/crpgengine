typedef struct {
    LogLevel level;
} Log;

char *getLogLevelStr(LogLevel logLevel) {
    if (logLevel == DEBUG) {
        return "debug";
    } else if (logLevel == NORMAL) {
        return "normal";
    } else {
        return "unknown";
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
        sprintf(toLog, "[%s - %s] %s\n", s, getLogLevelStr(logLevel), message);
        vprintf(toLog, ag);
    }
}

void addDebug(Log *log, char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(log, DEBUG, message, args);
    va_end(args);
}

void addLog(Log *log, char *message, ...) {
    va_list args;
    va_start(args, message);
    addLogWithLevel(log, NORMAL, message, args);
    va_end(args);
}
