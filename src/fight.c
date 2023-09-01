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
    fight->menu = 0;
    return fight;
}

void cancelFight(Fight *fight) {
    fight->beastCount = 0;
}

int isFightDone(Fight *fight) {
    return fight->beastCount == 0;
}

void processFightAnimations() {
    // stub
}

int getActionGaugeRaise(double elapsedTime, int dexterity) {
    int amount = (int) (elapsedTime + dexterity) / 10;
    return amount > MAX_ACTION_GAUGE_RAISE ? MAX_ACTION_GAUGE_RAISE : amount;
}

int normalizeActionGauge(int current, int amount) {
    int total = current + amount;
    return total > MAX_ACTION_GAUGE ? MAX_ACTION_GAUGE : total;
}

void fightUpdate(Fight *fight) {
    double end = getTimeInMS();
    double interval = end - fight->time;
    for (int i = 0; i < fight->beastCount; i++) {
        Beast *b = fight->beasts[i];
        int amountToRaise = getActionGaugeRaise(interval, b->attributes->dexterity);
        if (b->actionGauge < MAX_ACTION_GAUGE) {
            b->actionGauge = normalizeActionGauge(b->actionGauge, amountToRaise);
        }
    }
    for (int i = 0; i < fight->player->partyCount; i++) {
        Mobile *mob = fight->player->party[i];
        int amountToRaise = getActionGaugeRaise(interval, calculateMobileAttributes(mob)->dexterity);
        if (!isReadyForAction(mob) && mob->hp > 0) {
            mob->actionGauge = normalizeActionGauge(mob->actionGauge, amountToRaise);
            if (isReadyForAction(mob) && fight->cursors[fight->menu] == -1) {
                fight->cursors[fight->menu] = i;
            }
        }
    }
    fight->time = end;
}

Vector2 getFightPlayerPosition(int playerNumber) {
    return (Vector2) {
            FIGHT_PLAYER_X,
            (float) (FIGHT_PLAYER_Y_MARGIN + (MOB_HEIGHT * playerNumber) + (FIGHT_PLAYER_Y_PADDING * playerNumber))
    };
}
