typedef struct When {
    Mobile *source;
    int condition;
    Mobile *mobileTrigger;
    char *story;
} When;

typedef struct Then {
    Mobile *target;
    char *message;
    char *story;
    int outcome;
} Then;

typedef struct ControlBlock {
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

When *createWhen() {
    When *when = malloc(sizeof(When));
    when->mobileTrigger = NULL;
    when->source = NULL;
    return when;
}

Then *createThen() {
    Then *then = malloc(sizeof(Then));
    then->target = NULL;
    return then;
}

int needsToRemoveActiveControlBlock(ControlBlock *control) {
    return control != NULL && control->progress >= control->thenCount;
}
