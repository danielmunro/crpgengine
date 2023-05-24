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
    int control;
    When *when[MAX_INSTRUCTIONS];
    Then *then[MAX_INSTRUCTIONS];
    int whenCount;
    int thenCount;
    int progress;
} ControlBlock;

ControlBlock *createControlBlock(int control) {
    ControlBlock *cb = malloc(sizeof(ControlBlock));
    cb->control = control;
    cb->whenCount = 0;
    cb->thenCount = 0;
    cb->progress = 0;
    return cb;
}

typedef struct ControlBlockInt {
    int control;
    char *when[MAX_INSTRUCTIONS][2];
    char *then[MAX_INSTRUCTIONS][2];
    int whenCount;
    int thenCount;
} ControlBlockInt;

ControlBlockInt *createControlBlockInt(int control) {
    ControlBlockInt *cb = malloc(sizeof(ControlBlockInt));
    cb->control = control;
    cb->whenCount = 0;
    cb->thenCount = 0;
    return cb;
}
