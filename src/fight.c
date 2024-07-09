#include <mm_malloc.h>
#include "headers/beastiary.h"
#include "headers/graphics/ui/ui.h"
#include "headers/action.h"
#include "headers/player.h"

typedef struct {
    Beast *beast;
    int max;
} BeastEncounter;

typedef struct {
    Texture2D background;
    const char *backgroundFilePath;
    BeastEncounter *beastEncounters[MAX_BEASTS];
    int beastEncountersCount;
} Encounters;

typedef struct {
    Context *context;
    MenuType menu;
    Beast **beasts;
    Player *player;
    Action **actions;
    int actionCount;
    bool mobsDefending[MAX_PARTY_SIZE];
    bool beastsDefending[MAX_BEASTS_IN_FIGHT];
    int beastCount;
    int *cursors;
    double time;
    bool cancelled;
    double cancelledAt;
} Fight;

Encounters *createEncounters() {
    Encounters *e = malloc(sizeof(Encounters));
    e->beastEncountersCount = 0;
    e->backgroundFilePath = NULL;
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

void addAction(Fight *fight, Action *action) {
    fight->actions[fight->actionCount] = action;
    if (action->initiator->mob != NULL) {
        action->initiator->mob->step = action->step;
    }
    fight->actionCount++;
}

void removeAction(Fight *fight) {
    for (int i = 1; i < fight->actionCount; i++) {
        fight->actions[i - 1] = fight->actions[i];
    }
    fight->actionCount--;
}

void cancelFight(Fight *fight) {
    fight->cancelled = true;
    fight->cancelledAt = getTimeInMS();
}

int isFightDone(const Fight *fight) {
    return fight->beastCount == 0 || fight->cancelled;
}

Vector2 getFightPlayerPosition(int playerNumber, int mobileHeight, Resolution r) {
    return (Vector2) {
            (float) (r.width * 0.8),
            (float) (FIGHT_PLAYER_Y_MARGIN
                     + (mobileHeight * playerNumber)
                     + (FIGHT_PLAYER_Y_PADDING * playerNumber))
    };
}
