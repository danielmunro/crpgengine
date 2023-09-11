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
    Action **actions;
    int actionCount;
    bool defending[MAX_PARTY_SIZE];
    int beastCount;
    int *cursors;
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
        Beast **beasts,
        Player *player,
        int beastCount) {
    Fight *fight = malloc(sizeof(Fight));
    fight->beastCount = beastCount;
    fight->beasts = calloc(beastCount, sizeof(Beast));
    for (int i = 0; i < beastCount; i++) {
        fight->beasts[i] = beasts[i];
    }
    fight->player = player;
    fight->actions = calloc(MAX_ACTIONS, sizeof(Action));
    fight->time = getTimeInMS();
    fight->cursors = calloc(MAX_CURSORS, sizeof(int));
    for (int i = 0; i < MAX_CURSORS; i++) {
        fight->cursors[i] = -1;
    }
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        fight->defending[i] = false;
    }
    fight->menu = 0;
    fight->actionCount = 0;
    return fight;
}

void addAction(Fight *fight, Action *action) {
    fight->actions[fight->actionCount] = action;
    fight->actionCount++;
}

void removeAction(Fight *fight) {
    for (int i = 1; i < fight->actionCount; i++) {
        fight->actions[i - 1] = fight->actions[i];
    }
    fight->actionCount--;
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

Vector2 getFightPlayerPosition(int playerNumber, int mobileHeight) {
    return (Vector2) {
            (float) (ui->screen->width * 0.8),
            (float) (FIGHT_PLAYER_Y_MARGIN
                     + (mobileHeight * playerNumber)
                     + (FIGHT_PLAYER_Y_PADDING * playerNumber))
    };
}
