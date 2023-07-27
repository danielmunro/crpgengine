typedef struct {
    Mobile *source;
    int condition;
    Mobile *trigger;
    const char *story;
} When;

typedef struct {
    Mobile *target;
    const char *message;
    const char *story;
    const char *direction;
    int outcome;
    Vector2 position;
    bool parallel;
} Then;

typedef struct {
    When *when[MAX_INSTRUCTIONS];
    Then *then[MAX_INSTRUCTIONS];
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
    for (int i = 0; i < MAX_INSTRUCTIONS; i++) {
        cb->thensComplete[i] = false;
    }
    return cb;
}

When *createWhen(
        Mobile *source,
        Mobile *trigger,
        int condition,
        const char *story) {
    When *when = malloc(sizeof(When));
    when->source = source;
    when->trigger = trigger;
    when->condition = condition;
    when->story = story;
    return when;
}

Then *createThen(Mobile *target, const char *message, const char *story, const char *direction, const Outcome outcome, Vector2 position, bool parallel) {
    Then *then = malloc(sizeof(Then));
    then->target = target;
    then->message = message;
    then->direction = direction;
    then->story = story;
    then->outcome = outcome;
    then->position = position;
    then->parallel = parallel;
    return then;
}

bool hasConditionEngaged(Player *p, int condition, Mobile *mobileTrigger) {
    return condition == ENGAGED && isSpeakingTo(p, mobileTrigger);
}

bool hasConditionStory(Player *p, int condition, const char *story) {
    return condition == HAS_STORY && hasStory(p, story);
}

bool hasConditionNoStory(Player *p, int condition, const char *story) {
    return condition == NOT_HAS_STORY && !hasStory(p, story);
}

bool isWhenActivated(Player *p, When *when) {
    return hasConditionEngaged(p, when->condition, when->trigger)
           || hasConditionStory(p, when->condition, when->story)
           || hasConditionNoStory(p, when->condition, when->story);
}

bool areConditionsMet(ControlBlock *cb, Player *p) {
    for (int c = 0; c < cb->whenCount; c++) {
        if (!isWhenActivated(p, cb->when[c])) {
            addDebug(p->log, "conditions not met");
            return false;
        }
    }
    addDebug(p->log, "conditions met");
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
    return then->outcome == DIRECTION;
}

bool isSpeakOutcome(Then *then) {
    return then->outcome == SPEAK;
}

bool needsToStartMoving(Then *then) {
    return then->outcome == MOVE_TO && !isMoving(then->target);
}