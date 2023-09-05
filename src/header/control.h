#ifndef CJRPGENGINE_CONTROL_H
#define CJRPGENGINE_CONTROL_H

#define MAX_CONTROLS 255
#define MAX_ACTIVE_CONTROLS 64

const char *conditions[] = {
        "engaged",
        "has_story",
        "not_has_story",
        "scene_loaded",
        "arrive_at",
};

typedef enum {
    ENGAGED = 0,
    HAS_STORY,
    NOT_HAS_STORY,
    SCENE_LOADED,
    ARRIVE_AT,
} Condition;

const char *outcomes[] = {
        "speak",
        "move_to",
        "set_direction",
        "sprite",
        "wait",
        "give_item",
        "take",
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
    TAKE,
    ADD_STORY,
    SET_POSITION,
    LOCK,
    UNLOCK,
    SAVE,
} Outcome;

#endif //CJRPGENGINE_CONTROL_H
