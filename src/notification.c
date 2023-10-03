typedef struct {
    NotificationType type;
    const char *message;
    Rectangle rect;
    int decay;
} Notification;

typedef struct {
    Notification **notifications;
    int count;
    double timeSinceUpdateInMs;
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
    nm->timeSinceUpdateInMs = timeInterval;
    if (nm->timeSinceUpdateInMs > MILLISECONDS_IN_SECOND) {
        if (nm->notifications[0] != NULL) {
            nm->notifications[0]->decay--;
            if (nm->notifications[0]->decay == 0) {
                free(nm->notifications[0]);
                nm->notifications[0] = NULL;
                nm->slideDown = NOTIFICATION_HEIGHT + ui->menu->padding;
            }
        }
        nm->timeSinceUpdateInMs -= MILLISECONDS_IN_SECOND;
    }
    if (nm->slideDown > 0) {
        float amount = (float) (nm->timeSinceUpdateInMs / 100);
        nm->slideDown -= amount;
        if (nm->slideDown < 0) {
            nm->slideDown = 0;
            if (nm->notifications[0] == NULL && nm->notifications[1]) {
                for (int i = 1; i < nm->count + 1; i++) {
                    nm->notifications[i - 1] = nm->notifications[i];
                }
                nm->count--;
            }
        }
    }
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
            rect.x += (float) (nm->timeSinceUpdateInMs / 2);
        }
        if (nm->notifications[0] == NULL && nm->notifications[1] != NULL && nm->slideDown > 0) {
            rect.y += NOTIFICATION_HEIGHT + ui->menu->padding - nm->slideDown;
        }
        if (nm->notifications[i] != NULL) {
            drawMenuRect(rect);
            drawTextInArea(nm->notifications[i]->message, rect, font);
        }
    }
}
