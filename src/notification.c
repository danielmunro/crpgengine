typedef struct {
    NotificationType type;
    const char *message;
    Rectangle rect;
    int decay;
} Notification;

typedef struct {
    Notification **notifications;
    int count;
    double timeSinceUpdate;
    float slideDown;
} NotificationManager;

NotificationManager *createNotificationManager() {
    NotificationManager *nm = malloc(sizeof(NotificationManager));
    nm->notifications = calloc(MAX_NOTIFICATIONS, sizeof(Notification));
    nm->count = 0;
    nm->slideDown = 0;
    return nm;
}

Notification *createNotification(NotificationType type, const char *message) {
    Notification *notification = malloc(sizeof(Notification));
    notification->type = type;
    notification->message = message;
    notification->decay = NOTIFICATION_DECAY_SECONDS;
    return notification;
}

void addNotification(NotificationManager *nm, Notification *n) {
    nm->notifications[nm->count] = n;
    nm->count++;
}

void decayNotifications(NotificationManager *nm, double timeInterval) {
    nm->timeSinceUpdate = timeInterval;
    if (nm->timeSinceUpdate > 1000.0) {
        if (nm->notifications[0] != NULL) {
            nm->notifications[0]->decay--;
            if (nm->notifications[0]->decay == 0) {
                free(nm->notifications[0]);
                nm->notifications[0] = NULL;
                nm->slideDown = NOTIFICATION_HEIGHT;
                for (int i = 1; i < nm->count + 1; i++) {
                    nm->notifications[i - 1] = nm->notifications[i];
                }
                nm->count--;
            }
        }
        nm->timeSinceUpdate -= 1000.0;
    }
//    if (nm->slideDown > 0) {
//        float amount = (float) (nm->timeSinceUpdate / 100);
//        for (int i = 0; i < nm->count; i++) {
//            if (nm->notifications[i] != NULL) {
//                nm->notifications[i]->rect.y += amount;
//            }
//        }
//        nm->slideDown -= amount;
//        if (nm->slideDown < 0) {
//            nm->slideDown = 0;
//        }
//    }
}

void drawNotifications(NotificationManager *nm, FontStyle *font) {
    for (int i = 0; i < nm->count; i++) {
        Rectangle rect = (Rectangle) {
            (float) ui->screen->width
                - NOTIFICATION_WIDTH
                - ui->menu->padding,
            (float) ((float) ui->screen->height
                - (ui->menu->padding * (float) (i + 1))
                - (float) (NOTIFICATION_HEIGHT * (i + 1))),
            NOTIFICATION_WIDTH,
            NOTIFICATION_HEIGHT,
        };
        if (i == 0 && nm->notifications[0] != NULL && nm->notifications[0]->decay <= 1) {
            rect.x += (float) (nm->timeSinceUpdate / 2);
        }
        drawMenuRect(rect);
        drawTextInArea(nm->notifications[i]->message, rect, font);
    }
}
