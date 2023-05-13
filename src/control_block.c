typedef struct ControlBlock {
    char *condition;
    char *test;
    char *when[MAX_INSTRUCTIONS][2];
    char *then[MAX_INSTRUCTIONS][2];
    int whenCount;
    int thenCount;
} ControlBlock;

ControlBlock *createControlBlock(char *condition, char *test) {
    ControlBlock *cb = malloc(sizeof(ControlBlock));
    cb->condition = condition;
    cb->test = test;
    cb->whenCount = 0;
    cb->thenCount = 0;
    return cb;
}
