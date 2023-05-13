typedef struct ControlBlock {
    char *condition;
    char *test;
    char *instructions[MAX_INSTRUCTIONS][2];
    int instructionCount;
} ControlBlock;

ControlBlock *createControlBlock(char *condition, char *test) {
    ControlBlock *cb = malloc(sizeof(ControlBlock));
    cb->condition = condition;
    cb->test = test;
    cb->instructionCount = 0;
    return cb;
}
