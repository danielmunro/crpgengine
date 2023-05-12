typedef struct ControlBlock {
    char *condition;
    char *test;
    char *instructions[MAX_INSTRUCTIONS][2];
} ControlBlock;

ControlBlock *createControlBlock(char *condition, char *test) {
    ControlBlock *cb = malloc(sizeof(ControlBlock));
    cb->condition = condition;
    cb->test = test;
    return cb;
}
