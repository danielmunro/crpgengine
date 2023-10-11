#ifndef CJRPGENGINE_CONTROL_H
#define CJRPGENGINE_CONTROL_H

#define MAX_CONTROLS 255
#define MAX_ACTIVE_CONTROLS 64
#define MAX_INSTRUCTIONS 255

const char *conditions[] = {
        "engaged",
        "has_story",
        "not_has_story",
        "has_item",
        "not_has_item",
        "scene_loaded",
        "arrive_at",
};

typedef enum {
    ENGAGED,
    HAS_STORY,
    NOT_HAS_STORY,
    HAS_ITEM,
    NOT_HAS_ITEM,
    SCENE_LOADED,
    ARRIVE_AT,
} Condition;

#define OUTCOME_DIALOG "dialog"

const char *outcomes[] = {
        OUTCOME_DIALOG,
        "move_to",
        "set_direction",
        "sprite",
        "wait",
        "give_item",
        "lose_item",
        "add_story",
        "set_position",
        "lock",
        "unlock",
        "save",
};

typedef enum {
    SPEAK = 0,
    MOVE_TO,
    SET_DIRECTION,
    SPRITE,
    WAIT,
    GIVE_ITEM,
    LOSE_ITEM,
    ADD_STORY,
    SET_POSITION,
    LOCK,
    UNLOCK,
    SAVE,
} Outcome;

typedef enum {
    EVENT_GAME_LOOP,
    EVENT_SCENE_LOADED,
} EventType;

Condition mapCondition(const char *when) {
    int count = sizeof(conditions) / sizeof(char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(conditions[i], when) == 0) {
            return i;
        }
    }
    return -1;
}

Outcome mapOutcome(const char *then) {
    int count = sizeof(outcomes) / sizeof(char *);
    for (int i = 0; i < count; i++) {
        if (strcmp(outcomes[i], then) == 0) {
            return i;
        }
    }
    return -1;
}

#include "control.c"

#endif //CJRPGENGINE_CONTROL_H
