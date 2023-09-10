#ifndef CJRPGENGINE_NOTIFICATION_H
#define CJRPGENGINE_NOTIFICATION_H

#define MAX_NOTIFICATIONS 64

typedef enum {
    RECEIVE_QUEST_ITEM,
    LOSE_QUEST_ITEM,
    RECEIVE_EXPERIENCE,
    RECEIVE_GOLD,
    LOSE_GOLD,
    SAVED,
} NotificationType;

#endif //CJRPGENGINE_NOTIFICATION_H