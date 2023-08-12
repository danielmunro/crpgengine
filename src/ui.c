typedef struct {
    Rectangle area;
    int cursor;
} TextBox;

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
    notification->message = &message[0];
    notification->decay = NOTIFICATION_DECAY_SECONDS;
    return notification;
}

void addNotification(NotificationManager *nm, Notification *n) {
    for (int i = 0; i < MAX_NOTIFICATIONS; i++) {
        if (nm->notifications[i] == NULL) {
            n->rect = (Rectangle) {
                    SCREEN_WIDTH - 300 - UI_PADDING,
                    SCREEN_HEIGHT - 200 - (80 * i),
                    300,
                    NOTIFICATION_HEIGHT,
            };
            nm->notifications[i] = n;
            nm->count++;
            return;
        }
    }
}

TextBox *createTextBox(Rectangle area) {
    TextBox *textBox = malloc(sizeof(TextBox));
    textBox->area = area;
    textBox->cursor = 0;
    return textBox;
}

void decayNotifications(NotificationManager *nm, double timeInterval) {
    nm->timeSinceUpdate += timeInterval;
    if (nm->timeSinceUpdate > 1000) {
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
        nm->timeSinceUpdate = 1000 - nm->timeSinceUpdate;
    }
    if (nm->notifications[0] != NULL && nm->notifications[0]->decay <= 1) {
        nm->notifications[0]->rect.x += (float) (nm->timeSinceUpdate / 100);
    }
    if (nm->slideDown > 0) {
        float amount = (float) (nm->timeSinceUpdate / 100);
        for (int i = 0; i < nm->count; i++) {
            if (nm->notifications[i] != NULL) {
                nm->notifications[i]->rect.y += amount;
            }
        }
        nm->slideDown -= amount;
        if (nm->slideDown < 0) {
            nm->slideDown = 0;
        }
    }
}

void drawBlueBox(Rectangle rect) {
    DrawRectangleGradientH(
            (int) rect.x,
            (int) rect.y,
            (int) rect.width,
            (int) rect.height,
            BLUE,
            DARKBLUE
    );
}

Rectangle drawSaveBox() {
    float marginX = (float) SCREEN_WIDTH / 10, marginY = (float) SCREEN_HEIGHT / 10;
    Rectangle alertBox = {
            marginX,
            marginY,
            SCREEN_WIDTH - (marginX * 2),
            SCREEN_HEIGHT - (marginY * 2)};
    drawBlueBox(alertBox);
    return alertBox;
}

Rectangle drawAlertBox() {
    float marginX = (float) SCREEN_WIDTH / 5, marginY = (float) SCREEN_HEIGHT / 5;
    Rectangle alertBox = {
            marginX,
            marginY,
            SCREEN_WIDTH - (marginX * 2),
            SCREEN_HEIGHT - (marginY * 2)};
    drawBlueBox(alertBox);
    return alertBox;
}

Rectangle drawInGameMenuBox() {
    Rectangle rect = (Rectangle) {0, 0, SCREEN_WIDTH, SCREEN_HEIGHT};
    drawBlueBox(rect);
    return rect;
}

void drawText(const char *message, Vector2D position) {
    DrawText(&message[0], position.x, position.y, FONT_SIZE, WHITE);
}

void drawDialogBox(const char *message) {
    drawBlueBox((Rectangle) {0, SCREEN_HEIGHT - 150, SCREEN_WIDTH, SCREEN_HEIGHT});
    unsigned long lines = (strlen(message) / MAX_CHARACTERS_PER_LINE) + 1;
    int startY = SCREEN_HEIGHT - 135;
    for (int i = 0; i < lines; i++) {
        char line[MAX_CHARACTERS_PER_LINE + 1];
        memcpy(line, &message[i * MAX_CHARACTERS_PER_LINE], MAX_CHARACTERS_PER_LINE);
        line[MAX_CHARACTERS_PER_LINE] = '\0';
        drawText(&line[0], (Vector2D) {15, startY + (LINE_HEIGHT * i)});
    }
}

int line(int line) {
    return line * LINE_HEIGHT;
}

void drawInTextBox(TextBox *textBox, const char *text) {
    drawText(text, (Vector2D) {
            (int) textBox->area.x + UI_PADDING,
            (int) textBox->area.y + line(textBox->cursor) + UI_PADDING
    });
    textBox->cursor++;
}
