typedef struct {
    Log *log;
    UIManager *ui;
    Fight *fight;
    Menu **menus;
    MenuContext *menuContext;
} FightManager;

FightManager *createFightManager(Log *log, UIManager *ui) {
    FightManager *f = malloc(sizeof(FightManager));
    f->log = log;
    f->ui = ui;
    f->fight = NULL;
    f->menus = calloc(MAX_MENUS, sizeof(Menu));
    addMenu(f->menus, findMenu(ui->menus, BEAST_LIST_FIGHT_MENU));
    addMenu(f->menus, findMenu(ui->menus, MOBILE_SELECT_FIGHT_MENU));
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
        Spritesheet *menuSprite,
        MenuContext *mc) {
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
    mc->fight = fight;
    f->menuContext = mc;
}

void checkRemoveFight(FightManager *f) {
    if (isFightDone(f->fight)) {
        free(f->menuContext);
        f->menuContext = NULL;
        free(f->fight);
        f->fight = NULL;
    }
}

int getNextCursorPosition(FightManager *fm) {
    Menu *m = getCurrentMenu(fm->menus);
    Player *p = fm->fight->player;
    for (int i = m->cursor + 1; i < m->getCursorLength(fm->menuContext); i++) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    for (int i = 0; i < m->cursor; i++) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    return m->cursor;
}

int getPreviousCursorPosition(FightManager *fm) {
    Menu *m = getCurrentMenu(fm->menus);
    Player *p = fm->fight->player;
    int cursorLength = m->getCursorLength(fm->menuContext) - 1;
    for (int i = m->cursor - 1; i >= 0; i--) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    for (int i = cursorLength; i > m->cursor; i--) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    return m->cursor;
}

void fightSpaceKeyPressed(FightManager *fm) {
    Menu *currentMenu = getCurrentMenu(fm->menus);
    int c = currentMenu->cursor;
    if (c > -1) {
        currentMenu->cursor = getNextCursorPosition(fm);
        if (currentMenu->type == MOBILE_SELECT_FIGHT_MENU) {
//            fm->fight->player->party[c]->actionGauge = 0;
            addMenu(fm->menus, findMenu(fm->ui->menus, ACTION_SELECT_FIGHT_MENU));
        }
//        if (fight->cursors[fight->menu] == c) {
//            fight->cursors[fight->menu] = -1;
//        }
    }
}

void checkFightInput(FightManager *fm) {
    addDebug(fm->log, "fight -- check player input");
    if (IsKeyPressed(KEY_DOWN)) {
        getCurrentMenu(fm->menus)->cursor = getNextCursorPosition(fm);
    }
    if (IsKeyPressed(KEY_UP)) {
        getCurrentMenu(fm->menus)->cursor = getPreviousCursorPosition(fm);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        fightSpaceKeyPressed(fm);
    }
    if (IsKeyPressed(KEY_ESCAPE)) {
        cancelFight(fm->fight);
    }
}
