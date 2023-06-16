typedef struct {
    LogLevel level;
} Log;

Log *createLog(LogLevel configuredLogLevel) {
    Log *log = malloc(sizeof(Log));
    log->level = configuredLogLevel;
    return log;
}

void addLog(Log *log, LogLevel logLevel, char *message, ...) {
    if (log->level >= logLevel) {
        char *toLog = (char *)malloc(strlen(message) + 3);
        strcat(toLog, message);
        strcat(toLog, "\n");
        va_list args;
        va_start(args, message);
        vprintf(toLog, args);
        va_end(args);
    }
}
