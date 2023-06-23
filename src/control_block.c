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
    int outcome;
} Then;

typedef struct {
    When *when[MAX_INSTRUCTIONS];
    Then *then[MAX_INSTRUCTIONS];
    int whenCount;
    int thenCount;
    int progress;
} ControlBlock;

ControlBlock *createControlBlock() {
    ControlBlock *cb = malloc(sizeof(ControlBlock));
    cb->whenCount = 0;
    cb->thenCount = 0;
    cb->progress = 0;
    return cb;
}

When *createWhen(Mobile *source, Mobile *trigger, int condition, const char *story) {
    When *when = malloc(sizeof(When));
    when->source = source;
    when->trigger = trigger;
    when->condition = condition;
    when->story = story;
    return when;
}

Then *createThen(Mobile *target, const char *message, const char *story, const Outcome outcome) {
    Then *then = malloc(sizeof(Then));
    then->target = target;
    then->message = message;
    then->story = story;
    then->outcome = outcome;
    return then;
}

int needsToRemoveActiveControlBlock(ControlBlock *control) {
    return control != NULL && control->progress >= control->thenCount;
}
