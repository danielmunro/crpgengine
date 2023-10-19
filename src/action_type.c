typedef enum {
    ATTACK,
    CAST,
    ITEM,
    RUN,
    NONE,
} ActionType;

typedef enum {
    STEP_NONE,
    ATTACK_QUEUE,
    ATTACK_STEP_OUT,
    ATTACK_ACTION,
    ATTACK_RETURN,
    ACTION_STEP_RUN,
} ActionStep;
