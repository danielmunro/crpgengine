typedef struct {
    Mobile *mob;
    Beast *beast;
    int beastIndex;
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
    a->beastIndex = -1;
    return a;
}

ActionParticipant *createBeastParticipant(Beast *beast, int index) {
    ActionParticipant *a = malloc(sizeof(ActionParticipant));
    a->mob = NULL;
    a->beast = beast;
    a->beastIndex = index;
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

void setParticipantStep(ActionParticipant *participant, ActionStep step) {
    if (participant->mob != NULL) {
        participant->mob->step = step;
    } else {
        participant->beast->step = step;
    }
}

void setParticipantActionGauge(ActionParticipant *participant, int actionGauge) {
    if (participant->mob != NULL) {
        participant->mob->actionGauge = actionGauge;
    } else {
        participant->beast->actionGauge = actionGauge;
    }
}

ActionStep getParticipantActionStep(ActionParticipant *participant) {
    if (participant->mob != NULL) {
        return participant->mob->step;
    } else {
        return participant->beast->step;
    }
}

void finishAction(Action *action) {
    setParticipantActionGauge(action->initiator, 0);
}
