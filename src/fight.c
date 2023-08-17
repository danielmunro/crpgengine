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
    Vector2 cursors[MAX_CURSORS];
    Log *log;
    double time;
    FontStyle *activeFont;
    FontStyle *disabledFont;
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
        int count,
        Beast *beasts[MAX_BEASTS_IN_FIGHT],
        Font font) {
    Fight *fight = malloc(sizeof(Fight));
    fight->beastCount = count;
    fight->log = log;
    for (int i = 0; i < count; i++) {
        fight->beasts[i] = beasts[i];
    }
    fight->time = getTimeInMS();
    fight->activeFont = createDefaultFontStyle(font);
    fight->disabledFont = createDefaultDisabledFontStyle(font);
    return fight;
}

Fight *createFightFromEncounters(Log *log, Encounters *encounters, Font font) {
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
    Fight *fight = createFight(log, created, beasts, font);
    fight->beastCount = created;
    addDebug(log, "fight encountered with %d opponents", fight->beastCount);
    return fight;
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
            findAnimation(getPartyLeader(player)->animations, LEFT),
            (Vector2) {SCREEN_WIDTH * .8, 100});
}

void drawActionGauge(Rectangle rect, Color color) {
    DrawRectangleRounded(
            rect,
            (float) 1,
            4,
            color);
}

void drawActionGauges(Fight *fight, TextBox *textBox, Player *player) {
    for (int i = 0; i < player->partyCount; i++) {
        drawInMenuWithStyle(
                textBox,
                isReadyForAction(player->party[i]) ? fight->activeFont : fight->disabledFont,
                player->party[i]->name);
        drawActionGauge(
                (Rectangle) {
                        textBox->area.x + ACTION_GAUGE_X_OFFSET,
                        textBox->area.y + ACTION_GAUGE_Y_OFFSET + (float) (i * LINE_HEIGHT),
                        ACTION_GAUGE_WIDTH,
                        ACTION_GAUGE_HEIGHT,
                },
                GRAY);
        drawActionGauge(
                (Rectangle) {
                        textBox->area.x + ACTION_GAUGE_X_OFFSET,
                        textBox->area.y + ACTION_GAUGE_Y_OFFSET + (float) (i * LINE_HEIGHT),
                        ACTION_GAUGE_WIDTH * ((float) player->party[i]->actionGauge / MAX_ACTION_GAUGE),
                        ACTION_GAUGE_HEIGHT,
                },
                WHITE);
    }
}

void drawFightMenu(Fight *fight, Player *player, FontStyle *font) {
    TextBox *left = createTextBox(drawBottomLeftMenu(), font);
    TextBox *right = createTextBox(drawBottomRightMenu(), font);
    int count = fight->beastCount > MAX_MOB_NAMES_IN_FIGHT ? MAX_MOB_NAMES_IN_FIGHT : fight->beastCount;
    for (int i = 0; i < count; i++) {
        drawInMenu(left, fight->beasts[i]->name);
    }
    drawActionGauges(fight, right, player);
}

void drawFightView(Encounters *encounters, Fight *fight, Player *player, FontStyle *font) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawFightBackground(encounters);
    drawFightBeasts(fight);
    drawFightPlayer(player);
    drawFightMenu(fight, player, font);
    EndDrawing();
}

void fightSpaceKeyPressed(Fight *fight) {
    cancelFight(fight);
}

void checkFightInput(Fight *fight) {
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

void fightUpdate(Fight *fight, Player *player) {
    double end = getTimeInMS();
    double interval = end - fight->time;
    int amountToRaise = (int) interval / 10;
    for (int i = 0; i < fight->beastCount; i++) {
//        int amountToRaise = (int) interval / 10 + calcAttributesBeast(fight->beasts[i]).dexterity;
        if (fight->beasts[i]->actionGauge < MAX_ACTION_GAUGE) {
            fight->beasts[i]->actionGauge += amountToRaise;
        }
    }
    for (int i = 0; i < player->partyCount; i++) {
//        int amountToRaise = (int) (interval / 10) + calcAttributesMob(player->party[i]).dexterity;
        if (player->party[i]->actionGauge < MAX_ACTION_GAUGE) {
            player->party[i]->actionGauge += amountToRaise;
        }
    }
    fight->time = end;
}
