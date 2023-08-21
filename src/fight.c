typedef struct {
    Beast *beast;
    int max;
} BeastEncounter;

typedef struct {
    Texture2D background;
    BeastEncounter *beastEncounters[MAX_BEASTS];
    int beastEncountersCount;
} Encounters;

typedef struct {
    MenuType menu;
    Beast **beasts;
    Player *player;
    int beastCount;
    int *cursors;
    Log *log;
    double time;
} Fight;

Encounters *createEncounters() {
    Encounters *e = malloc(sizeof(Encounters));
    e->beastEncountersCount = 0;
    return e;
}

BeastEncounter *createBeastEncounter(Beast *beast, int max) {
    BeastEncounter *b = malloc(sizeof(BeastEncounter));
    b->beast = beast;
    b->max = max;
    return b;
}

BeastEncounter *createBeastEncounterFromData(Beast *beast, BeastEncounterData data) {
    return createBeastEncounter(beast, data.max);
}

Fight *createFight(
        Log *log,
        Beast **beasts,
        Player *player,
        int beastCount) {
    Fight *fight = malloc(sizeof(Fight));
    fight->log = log;
    fight->beastCount = beastCount;
    fight->beasts = calloc(beastCount, sizeof(Beast));
    for (int i = 0; i < beastCount; i++) {
        fight->beasts[i] = beasts[i];
    }
    fight->player = player;
    fight->time = getTimeInMS();
    fight->cursors = calloc(MAX_CURSORS, sizeof(int));
    for (int i = 0; i < MAX_CURSORS; i++) {
        fight->cursors[i] = -1;
    }
    return fight;
}

Fight *createFightFromEncounters(Log *log, Encounters *encounters, Player *player) {
    Beast *beasts[MAX_BEASTS_IN_FIGHT];
    int beastsToCreate = rand() % MAX_BEASTS_IN_FIGHT + 1;
    addDebug(log, "creating %d beasts for fight", beastsToCreate);
    int created = 0;
    while (created < beastsToCreate) {
        int e = rand() % encounters->beastEncountersCount + 0;
        int max = encounters->beastEncounters[e]->max;
        int amount = rand() % max + 1;
        if (amount > beastsToCreate - created) {
            amount = beastsToCreate - created;
        }
        for (int i = 0; i < amount; i++) {
            beasts[created] = cloneBeast(encounters->beastEncounters[e]->beast);
            created++;
        }
    }
    Fight *fight = createFight(log, beasts, player, created);
    fight->beastCount = created;
    addDebug(log, "fight encountered with %d opponents", fight->beastCount);
    return fight;
}

void cancelFight(Fight *fight) {
    fight->beastCount = 0;
}

int getNextCursorPosition(Fight *fight) {
    for (int i = fight->cursors[fight->menu] + 1; i < MAX_CURSORS; i++) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    for (int i = 0; i < fight->cursors[fight->menu]; i++) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    return fight->cursors[fight->menu];
}

int getPreviousCursorPosition(Fight *fight) {
    for (int i = fight->cursors[fight->menu] - 1; i >= 0; i--) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    for (int i = MAX_CURSORS - 1; i > fight->cursors[fight->menu]; i--) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    return fight->cursors[fight->menu];
}

void fightSpaceKeyPressed(Fight *fight) {
    int c = fight->cursors[fight->menu];
    if (c > -1) {
//        Player *p = fight->player;
//        p->party[c]->actionGauge = 0;
//        p->party[c]->hp -= 2;
//        p->party[c]->mana -= 1;
        fight->cursors[fight->menu] = getNextCursorPosition(fight);
        if (fight->menu == FIGHT_MENU_MOBILE_SELECT) {
            fight->menu = FIGHT_MENU_ACTION_SELECT;
        }
        if (fight->cursors[fight->menu] == c) {
            fight->cursors[fight->menu] = -1;
        }
    }
}

void checkFightInput(Fight *fight) {
    addDebug(fight->log, "fight -- check player input");
    if (IsKeyPressed(KEY_DOWN)) {
        fight->cursors[fight->menu] = getNextCursorPosition(fight);
    }
    if (IsKeyPressed(KEY_UP)) {
        fight->cursors[fight->menu] = getPreviousCursorPosition(fight);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        fightSpaceKeyPressed(fight);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        cancelFight(fight);
    }
}

int isFightDone(Fight *fight) {
    return fight->beastCount == 0;
}

void processFightAnimations() {
    // stub
}

int getActionGaugeRaise(double elapsedTime, int dexterity) {
    return (int) (elapsedTime + dexterity) / 10;
}

void fightUpdate(Fight *fight) {
    double end = getTimeInMS();
    double interval = end - fight->time;
    for (int i = 0; i < fight->beastCount; i++) {
        Beast *b = fight->beasts[i];
        int amountToRaise = getActionGaugeRaise(interval, b->attributes.dexterity);
        if (b->actionGauge < MAX_ACTION_GAUGE) {
            b->actionGauge += amountToRaise;
        }
    }
    for (int i = 0; i < fight->player->partyCount; i++) {
        Mobile *mob = fight->player->party[i];
        int amountToRaise = getActionGaugeRaise(interval, calculateAttributes(mob).dexterity);
        if (!isReadyForAction(mob)) {
            mob->actionGauge += amountToRaise;
            if (isReadyForAction(mob) && fight->cursors[fight->menu] == -1) {
                fight->cursors[fight->menu] = i;
            }
        }
    }
    fight->time = end;
}
