typedef struct {
    double start;
    double elapsedTime;
    double timeInterval;
    NotificationManager *notificationManager;
    Player *player;
    bool logMemory;
} Timing;

Timing *createTiming(NotificationManager *nm, Player *player, bool logMemory) {
    Timing *t = malloc(sizeof(Timing));
    t->elapsedTime = 0;
    t->timeInterval = 0;
    t->notificationManager = nm;
    t->player = player;
    t->logMemory = logMemory;
    return t;
}

void startTiming(Timing *t) {
    t->start = getTimeInMS();
}

void stopTiming(Timing *t) {
    double end =  getTimeInMS();
    t->elapsedTime += end - t->start;
    decayNotifications(t->notificationManager, t->elapsedTime);
    if (t->elapsedTime > 1000.0) {
        t->elapsedTime -= 1000.0;
        t->start = end;
        t->player->secondsPlayed += 1;
        if (t->logMemory) {
            reportMaxMemory();
        }
    }
}
