#include <mm_malloc.h>
#include "headers/util/util.h"
#include "headers/mobile.h"
#include "headers/player.h"

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
        "sleep",
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
    SLEEP,
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

typedef struct {
    int id;
    const char *name;
    RectangleD rect;
} ArriveAt;

typedef struct {
    Mobile *source;
    Condition condition;
    Mobile *trigger;
    const char *story;
    const char **items;
    int itemCount;
    ArriveAt *arriveAt;
} When;

typedef struct {
    Mobile *target;
    const char *message;
    const char *story;
    const char *direction;
    const char **items;
    int itemCount;
    Outcome outcome;
    int amount;
    Vector2D position;
    bool parallel;
} Then;

typedef struct {
    When **when;
    Then **then;
    int whenCount;
    int thenCount;
    int progress;
    bool thensComplete[MAX_INSTRUCTIONS];
} ControlBlock;

ControlBlock *createControlBlock() {
    ControlBlock *cb = malloc(sizeof(ControlBlock));
    cb->whenCount = 0;
    cb->thenCount = 0;
    cb->progress = 0;
    cb->when = calloc(MAX_INSTRUCTIONS, sizeof(When));
    cb->then = calloc(MAX_INSTRUCTIONS, sizeof(Then));
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        cb->thensComplete[i] = false;
    }
    return cb;
}

When *createWhen(
        Mobile *source,
        Mobile *trigger,
        int condition,
        const char *story,
        const char **items,
        int itemCount,
        ArriveAt *arriveAt) {
    When *when = malloc(sizeof(When));
    when->source = source;
    when->trigger = trigger;
    when->condition = condition;
    when->story = story;
    when->items = items;
    when->itemCount = itemCount;
    when->arriveAt = arriveAt;
    return when;
}

Then *createThen(
        Mobile *target,
        const char *message,
        const char *story,
        const char *direction,
        const char **items,
        int itemCount,
        const Outcome outcome,
        Vector2D position,
        bool parallel,
        int amount) {
    Then *then = malloc(sizeof(Then));
    then->target = target;
    then->message = message;
    then->direction = direction;
    then->items = items;
    then->itemCount = itemCount;
    then->story = story;
    then->outcome = outcome;
    then->position = position;
    then->parallel = parallel;
    then->amount = amount;
    return then;
}

ArriveAt *createArriveAt(int id, const char *name, RectangleD rect) {
    ArriveAt *a = malloc(sizeof(ArriveAt));
    a->id = id;
    a->name = name;
    a->rect = rect;
    return a;
}

bool hasConditionEngaged(const Player *p, Condition condition, const Mobile *mobileTrigger) {
    return condition == ENGAGED && isSpeakingTo(p, mobileTrigger);
}

bool hasConditionStory(const Player *p, Condition condition, const char *story) {
    return condition == HAS_STORY && hasStory(p, story);
}

bool hasConditionNoStory(const Player *p, Condition condition, const char *story) {
    return condition == NOT_HAS_STORY && !hasStory(p, story);
}

bool isSceneLoaded(Condition condition, EventType eventType) {
    return condition == SCENE_LOADED && eventType == EVENT_SCENE_LOADED;
}

bool hasArrivedAt(const Player *p, Condition condition, const ArriveAt *arriveAt) {
    if (arriveAt != NULL) {
        return condition == ARRIVE_AT
                && !isMoving(getPartyLeader(p))
                && CheckCollisionRecs(
                        getMobileRectangle(getPartyLeader(p)),
                        rectangleDtoRectangle(arriveAt->rect));
    }
    return false;
}

bool hasItem(const Player *p, Condition condition, const char **itemNames, int count) {
    if (itemNames == NULL) {
        return false;
    }
    int hasItems = 0;
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < p->itemCount; j++) {
            if (strcmp(p->items[j]->name, itemNames[i]) == 0) {
                hasItems++;
            }
        }
    }
    if (hasItems == count) {
        return condition == HAS_ITEM;
    }
    return condition == NOT_HAS_ITEM;
}

bool isWhenActivated(const Player *p, const When *when, EventType eventType) {
    return hasConditionEngaged(p, when->condition, when->trigger)
           || hasConditionStory(p, when->condition, when->story)
           || hasConditionNoStory(p, when->condition, when->story)
           || isSceneLoaded(when->condition, eventType)
           || hasArrivedAt(p, when->condition, when->arriveAt)
           || hasItem(p, when->condition, when->items, when->itemCount);
}

bool areConditionsMet(const ControlBlock *cb, const Player *p, EventType eventType) {
    for (int c = 0; c < cb->whenCount; c++) {
        if (!isWhenActivated(p, cb->when[c], eventType)) {
            return false;
        }
    }
    return true;
}

bool needsToRemoveActiveControlBlock(const ControlBlock *control) {
    return control != NULL && control->progress >= control->thenCount;
}

bool isMovingAndAtDestination(const ControlBlock *cb) {
    return cb->then[cb->progress]->outcome == MOVE_TO &&
           vector2DEquals(
                   cb->then[cb->progress]->target->position,
                   cb->then[cb->progress]->position
           );
}

bool isAddStoryOutcome(const Then *then) {
    return then->outcome == ADD_STORY;
}

bool isFaceDirectionOutcome(const Then *then) {
    return then->outcome == SET_DIRECTION;
}

bool isSpeakOutcome(const Then *then) {
    return then->outcome == SPEAK;
}

bool needsToChangePosition(const Then *then) {
    return then->outcome == SET_POSITION;
}

bool needsToStartMoving(const Then *then) {
    return then->outcome == MOVE_TO && !isMoving(then->target);
}

bool needsToWait(const Then *then) {
    return then->outcome == WAIT;
}

bool needsToLock(const Then *then) {
    return then->outcome == LOCK;
}

bool needsToUnlock(const Then *then) {
    return then->outcome == UNLOCK;
}

bool hasAmountProperty(ThenData thenData) {
    return strcmp(thenData.action, outcomes[WAIT]) == 0;
}

bool needsToSave(const Then *then) {
    return then->outcome == SAVE;
}

bool needsToReceiveItem(const Then *then, const Mobile *playerMob) {
    return then->outcome == GIVE_ITEM && then->target == playerMob;
}

bool needsToLoseItem(const Then *then, const Mobile *playerMob) {
    return then->outcome == LOSE_ITEM && then->target == playerMob;
}

bool needsToSleep(const Then *then, const Mobile *playerMob) {
    return then->outcome == SLEEP && then->target == playerMob;
}
