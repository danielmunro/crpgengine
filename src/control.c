typedef struct {
    const char *name;
    Rectangle rect;
} ArriveAt;

typedef struct {
    Mobile *source;
    Condition condition;
    Mobile *trigger;
    const char *story;
    ArriveAt *arriveAt;
} When;

typedef struct {
    Mobile *target;
    const char *message;
    const char *story;
    const char *direction;
    const char *item;
    Outcome outcome;
    int amount;
    Vector2 position;
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
        ArriveAt *arriveAt) {
    When *when = malloc(sizeof(When));
    when->source = source;
    when->trigger = trigger;
    when->condition = condition;
    when->story = story;
    when->arriveAt = arriveAt;
    return when;
}

Then *createThen(
        Mobile *target,
        const char *message,
        const char *story,
        const char *direction,
        const char *item,
        const Outcome outcome,
        Vector2 position,
        bool parallel,
        int amount) {
    Then *then = malloc(sizeof(Then));
    then->target = target;
    then->message = message;
    then->direction = direction;
    then->item = item;
    then->story = story;
    then->outcome = outcome;
    then->position = position;
    then->parallel = parallel;
    then->amount = amount;
    return then;
}

ArriveAt *createArriveAt(const char *name, Rectangle rect) {
    ArriveAt *a = malloc(sizeof(ArriveAt));
    a->name = name;
    a->rect = rect;
    return a;
}

bool hasConditionEngaged(Player *p, Condition condition, Mobile *mobileTrigger) {
    return condition == ENGAGED && isSpeakingTo(p, mobileTrigger);
}

bool hasConditionStory(Player *p, Condition condition, const char *story) {
    return condition == HAS_STORY && hasStory(p, story);
}

bool hasConditionNoStory(Player *p, Condition condition, const char *story) {
    return condition == NOT_HAS_STORY && !hasStory(p, story);
}

bool isSceneLoaded(Condition condition, EventType eventType) {
    return condition == SCENE_LOADED && eventType == EVENT_SCENE_LOADED;
}

bool hasArrivedAt(Player *p, Condition condition, ArriveAt *arriveAt) {
    if (arriveAt != NULL) {
        Rectangle c = GetCollisionRec(getMobileRectangle(getPartyLeader(p)), arriveAt->rect);
        return condition == ARRIVE_AT && (c.height > 0 || c.width > 0);
    }
    return false;
}

bool isWhenActivated(Player *p, When *when, EventType eventType) {
    return hasConditionEngaged(p, when->condition, when->trigger)
           || hasConditionStory(p, when->condition, when->story)
           || hasConditionNoStory(p, when->condition, when->story)
           || isSceneLoaded(when->condition, eventType)
           || hasArrivedAt(p, when->condition, when->arriveAt);
}

bool areConditionsMet(ControlBlock *cb, Player *p, EventType eventType) {
    for (int c = 0; c < cb->whenCount; c++) {
        if (!isWhenActivated(p, cb->when[c], eventType)) {
            addDebug("conditions not met");
            return false;
        }
    }
    addDebug("conditions met");
    return true;
}

bool needsToRemoveActiveControlBlock(ControlBlock *control) {
    return control != NULL && control->progress >= control->thenCount;
}

bool isMovingAndAtDestination(ControlBlock *cb) {
    return cb->then[cb->progress]->outcome == MOVE_TO &&
           vector2DEquals(
                   vector2DFromVect(cb->then[cb->progress]->target->position),
                   vector2DFromVect(cb->then[cb->progress]->position)
           );
}

bool isAddStoryOutcome(Then *then) {
    return then->outcome == ADD_STORY;
}

bool isFaceDirectionOutcome(Then *then) {
    return then->outcome == SET_DIRECTION;
}

bool isSpeakOutcome(Then *then) {
    return then->outcome == SPEAK;
}

bool needsToChangePosition(Then *then) {
    return then->outcome == SET_POSITION;
}

bool needsToStartMoving(Then *then) {
    return then->outcome == MOVE_TO && !isMoving(then->target);
}

bool needsToWait(Then *then) {
    return then->outcome == WAIT;
}

bool needsToLock(Then *then) {
    return then->outcome == LOCK;
}

bool needsToUnlock(Then *then) {
    return then->outcome == UNLOCK;
}

bool hasAmountProperty(ThenData thenData) {
    return strcmp(thenData.action, outcomes[WAIT]) == 0;
}

bool needsToSave(Then *then) {
    return then->outcome == SAVE;
}

bool needsToReceiveItem(Then *then, Mobile *playerMob) {
    return then->outcome == GIVE_ITEM && then->target == playerMob;
}
