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
    Beast *beasts[MAX_BEASTS_IN_FIGHT];
    int beastCount;
    Log *log;
} Fight;

Encounters *createEncounters() {
    Encounters *e = malloc(sizeof(Encounters));
    e->beastEncountersCount = 0;
    return e;
}

Fight *createFight(Log *log, int count, Beast *beasts[MAX_BEASTS_IN_FIGHT]) {
    Fight *fight = malloc(sizeof(Fight));
    fight->beastCount = count;
    fight->log = log;
    for (int i = 0; i < count; i++) {
        fight->beasts[i] = beasts[i];
    }
    return fight;
}

Fight *createFightFromEncounters(Log *log, Encounters *encounters) {
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
    Fight *fight = createFight(log, created, beasts);
    fight->beastCount = created;
    addDebug(log, "fight encountered with %d opponents", fight->beastCount);
    return fight;
}

BeastEncounter *createBeastEncounterFromData(Beast *beast, BeastEncounterData data) {
    BeastEncounter *beastEncounter = malloc(sizeof(BeastEncounter));
    beastEncounter->beast = beast;
    beastEncounter->max = data.max;
    return beastEncounter;
}

void cancelFight(Fight *fight) {
    fight->beastCount = 0;
}

void drawFightBackground(Encounters *encounters) {
    float scale = (float) SCREEN_WIDTH / (float) encounters->background.width;
    DrawTextureEx(encounters->background, (Vector2) {0, 0}, 0, scale, WHITE);
}

void drawFightBeasts(Fight *fight) {
    int width = 0;
    int height = 0;
    for (int i = 0; i < fight->beastCount; i++) {
        const int x = i % 3;
        DrawTextureEx(fight->beasts[i]->image,
                      (Vector2) {(float) width, (float) height},
                      0, SCALE, WHITE);
        width += fight->beasts[i]->image.width;
        if (x > 0 && x % 2 == 0) {
            height += fight->beasts[i]->image.height;
            width = 0;
        }
    }
}

void drawFightPlayer(Player *player) {
    drawAnimation(
            findAnimation(player->mob->animations, LEFT),
            (Vector2) {SCREEN_WIDTH * .8, 100 });
}

void drawFightView(Encounters *encounters, Fight *fight, Player *player) {
    ClearBackground(BLACK);
    drawFightBackground(encounters);
    drawFightBeasts(fight);
    drawFightPlayer(player);
}

void fightSpaceKeyPressed(Fight *fight) {
    cancelFight(fight);
}

void checkFightInput(Fight *fight, Player *player) {
    addDebug(fight->log, "fight -- check player input");
    if (IsKeyPressed(KEY_SPACE)) {
        fightSpaceKeyPressed(fight);
    }
}

int isFightDone(Fight *fight) {
    return fight->beastCount == 0;
}

void processFightAnimations() {
    // stub
}
