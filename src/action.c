typedef struct {
    Mobile *mob;
    Beast *beast;
} ActionParticipant;

typedef struct {
    Spell *spell;
    Item *item;
} ActionObject;

typedef struct {
    ActionParticipant *initiator;
    ActionParticipant *target;
    ActionObject *object;
    ActionType type;
    ActionStep step;
    double elapsedTime;
} Action;

ActionParticipant *createMobParticipant(Mobile *mob) {
    ActionParticipant *a = malloc(sizeof(ActionParticipant));
    a->mob = mob;
    a->beast = NULL;
    return a;
}

ActionParticipant *createBeastParticipant(Beast *beast) {
    ActionParticipant *a = malloc(sizeof(ActionParticipant));
    a->mob = NULL;
    a->beast = beast;
    return a;
}

ActionObject *createActionSpellObject(Spell *spell) {
    ActionObject *o = malloc(sizeof(ActionObject));
    o->item = NULL;
    o->spell = spell;
    return o;
}

ActionObject *createActionItemObject(Item *item) {
    ActionObject *o = malloc(sizeof(ActionObject));
    o->item = item;
    o->spell = NULL;
    return o;
}

Action *createAction(
        ActionType type,
        ActionStep step,
        ActionParticipant *initiator,
        ActionParticipant *target,
        ActionObject *object) {
    Action *action = malloc(sizeof(Action));
    action->type = type;
    action->step = step;
    action->initiator = initiator;
    action->target = target;
    action->object = object;
    action->elapsedTime = 0;
    return action;
}