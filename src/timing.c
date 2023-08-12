typedef struct {
    struct timeval start;
    double elapsedTime;
    double timeInterval;
    NotificationManager *notificationManager;
    Player *player;
    Log *log;
    bool logMemory;
} Timing;

Timing *createTiming(Log *log, NotificationManager *nm, Player *player, bool logMemory) {
    Timing *t = malloc(sizeof(Timing));
    t->elapsedTime = 0;
    t->timeInterval = 0;
    t->log = log;
    t->notificationManager = nm;
    t->player = player;
    t->logMemory = logMemory;
    return t;
}

void startTiming(Timing *t) {
    gettimeofday(&t->start, NULL);
}

void stopTiming(Timing *t) {
    struct timeval end;
    gettimeofday(&end, NULL);
    double timeInterval = (double) (end.tv_sec - t->start.tv_sec) * 1000.0;
    timeInterval += (end.tv_usec - t->start.tv_usec) / 1000.0;
    t->elapsedTime += timeInterval;
    if (t->elapsedTime > 1000.0) {
        t->elapsedTime -= 1000.0;
        t->player->secondsPlayed += 1;
        if (t->logMemory) {
            reportMaxMemory(t->log);
        }
    }
    decayNotifications(t->notificationManager, timeInterval);
}
