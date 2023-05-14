typedef struct When {
    Mobile *source;
    int condition;
    Mobile *target;
} When;

typedef struct Then {
    Mobile *source;
    int outcome;
    Mobile *target;

} Then;

typedef struct ControlBlock {
    int control;
    When *conditions[MAX_INSTRUCTIONS];
    Then *outcomes[MAX_INSTRUCTIONS];
} ControlBlock;

typedef struct ControlBlockInt {
    int control;
    char *when[MAX_INSTRUCTIONS][2];
    char *then[MAX_INSTRUCTIONS][2];
    int whenCount;
    int thenCount;
} ControlBlockInt;

ControlBlockInt *createControlBlock(int control) {
    ControlBlockInt *cb = malloc(sizeof(ControlBlockInt));
    cb->control = control;
    cb->whenCount = 0;
    cb->thenCount = 0;
    return cb;
}
