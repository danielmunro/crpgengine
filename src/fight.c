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
    Beast **beasts;
    Player *player;
    int beastCount;
    int *cursors;
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
        Beast **beasts,
        Player *player,
        int beastCount,
        Font font) {
    Fight *fight = malloc(sizeof(Fight));
    fight->log = log;
    fight->beastCount = beastCount;
    fight->beasts = calloc(beastCount, sizeof(Beast));
    for (int i = 0; i < beastCount; i++) {
        fight->beasts[i] = beasts[i];
    }
    fight->player = player;
    fight->time = getTimeInMS();
    fight->activeFont = createDefaultFontStyle(font);
    fight->disabledFont = createDefaultDisabledFontStyle(font);
    fight->cursors = calloc(MAX_CURSORS, sizeof(int));
    for (int i = 0; i < MAX_CURSORS; i++) {
        fight->cursors[i] = -1;
    }
    return fight;
}

Fight *createFightFromEncounters(Log *log, Encounters *encounters, Player *player, Font font) {
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
    Fight *fight = createFight(log, beasts, player, created, font);
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
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (player->party[i] != NULL) {
            drawAnimation(
                    findAnimation(getPartyLeader(player)->animations, LEFT),
                    (Vector2) {SCREEN_WIDTH * .8, (float) (100 + (MOB_HEIGHT * i))});
        }
    }
}

void drawActionGauge(Rectangle rect, Color color) {
    DrawRectangleRounded(
            rect,
            (float) 1,
            4,
            color);
}

void drawActionGauges(Fight *fight, TextBox *textBox) {
    for (int i = 0; i < fight->player->partyCount; i++) {
        drawInMenuWithStyle(
                textBox,
                isReadyForAction(fight->player->party[i]) ? fight->activeFont : fight->disabledFont,
                fight->player->party[i]->name);
        if (fight->cursors[FIGHT_CURSOR_MAIN] == i) {
            DrawRectangle(
                    (int) textBox->area.x + UI_PADDING,
                    (int) textBox->area.y + (LINE_HEIGHT * (i + 1)),
                    86,
                    2,
                    WHITE);
        }
        char hp[10];
        sprintf(hp, "%d", fight->player->party[i]->hp);
        drawText(
                hp,
                (Vector2) {
                    textBox->area.x + HP_X_OFFSET,
                    textBox->area.y + UI_PADDING + (float) (i * LINE_HEIGHT)
                    },
                    fight->activeFont);
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
                        ACTION_GAUGE_WIDTH * ((float) fight->player->party[i]->actionGauge / MAX_ACTION_GAUGE),
                        ACTION_GAUGE_HEIGHT,
                },
                WHITE);
    }
}

void drawFightMenu(Fight *fight, FontStyle *font) {
    TextBox *left = createTextBox(drawBottomLeftMenu(), font);
    TextBox *right = createTextBox(drawBottomRightMenu(), font);
    int count = fight->beastCount > MAX_MOB_NAMES_IN_FIGHT ? MAX_MOB_NAMES_IN_FIGHT : fight->beastCount;
    for (int i = 0; i < count; i++) {
        drawInMenu(left, fight->beasts[i]->name);
    }
    drawActionGauges(fight, right);
}

void drawFightView(Encounters *encounters, Fight *fight, FontStyle *font) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawFightBackground(encounters);
    drawFightBeasts(fight);
    drawFightPlayer(fight->player);
    drawFightMenu(fight, font);
    EndDrawing();
}

void fightSpaceKeyPressed(Fight *fight) {
    cancelFight(fight);
}

int getNextCursorPosition(Fight *fight, FightCursor cursor) {
    for (int i = fight->cursors[cursor] + 1; i < MAX_CURSORS; i++) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    for (int i = 0; i < fight->cursors[cursor]; i++) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    return -1;
}

int getPreviousCursorPosition(Fight *fight, FightCursor cursor) {
    for (int i = fight->cursors[cursor] - 1; i >= 0; i--) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    for (int i = MAX_CURSORS - 1; i > fight->cursors[cursor]; i--) {
        if (isReadyForAction(fight->player->party[i])) {
            return i;
        }
    }
    return -1;
}

void checkFightInput(Fight *fight) {
    addDebug(fight->log, "fight -- check player input");
    if (IsKeyPressed(KEY_DOWN)) {
        fight->cursors[FIGHT_CURSOR_MAIN] = getNextCursorPosition(fight, FIGHT_CURSOR_MAIN);
    }
    if (IsKeyPressed(KEY_UP)) {
        fight->cursors[FIGHT_CURSOR_MAIN] = getPreviousCursorPosition(fight, FIGHT_CURSOR_MAIN);
    }
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

void fightUpdate(Fight *fight) {
    double end = getTimeInMS();
    double interval = end - fight->time;
    for (int i = 0; i < fight->beastCount; i++) {
        Beast *b = fight->beasts[i];
        int amountToRaise = (int) (interval + b->attributes.dexterity) / 10;
        if (b->actionGauge < MAX_ACTION_GAUGE) {
            b->actionGauge += amountToRaise;
        }
    }
    for (int i = 0; i < fight->player->partyCount; i++) {
        Mobile *mob = fight->player->party[i];
        int amountToRaise = (int) (interval + calculateAttributes(mob).dexterity) / 10;
        if (!isReadyForAction(mob)) {
            mob->actionGauge += amountToRaise;
            if (isReadyForAction(mob) && fight->cursors[FIGHT_CURSOR_MAIN] == -1) {
                fight->cursors[FIGHT_CURSOR_MAIN] = i;
            }
        }
    }
    fight->time = end;
}
