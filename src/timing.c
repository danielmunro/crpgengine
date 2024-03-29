#include "headers/util/util.h"
#include "headers/notification.h"

typedef struct {
    double start;
    double elapsedTime;
    double timeInterval;
    NotificationManager *notificationManager;
    bool logMemoryUsage;
} Timing;

Timing *createTiming(NotificationManager *nm) {
    Timing *t = malloc(sizeof(Timing));
    t->elapsedTime = 0;
    t->timeInterval = 0;
    t->notificationManager = nm;
    t->logMemoryUsage = nm->context->game->logMemoryUsage;
    return t;
}

void startTiming(Timing *t) {
    t->start = getTimeInMS();
}

int stopTiming(Timing *t) {
    double end = getTimeInMS();
    t->elapsedTime += end - t->start;
    decayNotifications(t->notificationManager, t->elapsedTime);
    if (t->elapsedTime > 1000.0) {
        t->elapsedTime -= 1000.0;
        t->start = end;
        if (t->logMemoryUsage) {
            reportMaxMemory();
        }
        return 1;
    }
    return 0;
}
