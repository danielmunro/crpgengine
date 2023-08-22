typedef struct {
    Log *log;
    Fight *fight;
    Menu **menus;
    MenuContext *menuContext;
} FightManager;

FightManager *createFightManager(Log *log) {
    FightManager *f = malloc(sizeof(FightManager));
    f->log = log;
    f->fight = NULL;
    f->menus = calloc(MAX_MENUS, sizeof(Menu));
    f->menuContext = NULL;
    return f;
}

bool isFighting(FightManager *f) {
    return f->fight != NULL;
}

void createFightFromEncounters(
        FightManager *f,
        Log *log,
        Encounters *encounters,
        Player *player,
        Spritesheet *menuSprite) {
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
    Fight *fight = createFight(log, beasts, player, menuSprite, created);
    fight->beastCount = created;
    addDebug(log, "fight encountered with %d opponents", fight->beastCount);
    f->fight = fight;
}

void checkRemoveFight(FightManager *f) {
    if (isFightDone(f->fight)) {
        free(f->menuContext);
        f->menuContext = NULL;
        free(f->fight);
        f->fight = NULL;
    }
}
