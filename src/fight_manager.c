typedef struct {
    Log *log;
    UIManager *ui;
    Fight *fight;
    Menu **menus;
    SpellManager *spells;
} FightManager;

FightManager *createFightManager(Log *log, UIManager *ui, SpellManager *spells) {
    FightManager *f = malloc(sizeof(FightManager));
    f->log = log;
    f->ui = ui;
    f->spells = spells;
    f->fight = NULL;
    f->menus = calloc(MAX_MENUS, sizeof(Menu));
    addMenu(f->menus, findMenu(ui->menus, BEAST_LIST_FIGHT_MENU));
    addMenu(f->menus, findMenu(ui->menus, MOBILE_SELECT_FIGHT_MENU));
    return f;
}

bool isFighting(FightManager *f) {
    return f->fight != NULL;
}

Fight *createFightFromEncounters(
        FightManager *f,
        Log *log,
        Encounters *encounters,
        Player *player,
        Spritesheet *menuSprite) {
    Beast *beasts[MAX_BEASTS_IN_FIGHT];
    int beastsToCreate = rand() % MAX_BEASTS_IN_FIGHT + 1;
    addDebug(log, "creating %d beasts for fight", beastsToCreate);
    int created = 0;
    float x = BEAST_AREA.x;
    float y = BEAST_AREA.y;
    while (created < beastsToCreate) {
        int e = rand() % encounters->beastEncountersCount + 0;
        int max = encounters->beastEncounters[e]->max;
        int amount = rand() % max + 1;
        if (amount > beastsToCreate - created) {
            amount = beastsToCreate - created;
        }
        for (int i = 0; i < amount; i++) {
            beasts[created] = cloneBeast(encounters->beastEncounters[e]->beast);
            beasts[created]->position = (Rectangle) {
                    x, y, (float) beasts[created]->image.width, (float) beasts[created]->image.height,
            };
            float height = (float) beasts[created]->image.height;
            y += height + (float) (height * 0.5);
            if (y > BEAST_AREA.height) {
                y = BEAST_AREA.y;
                float width = (float) beasts[created]->image.width;
                x += width + (float) (width * 0.5);
            }
            created++;
        }
    }
    Fight *fight = createFight(log, beasts, player, menuSprite, created);
    fight->beastCount = created;
    addDebug(log, "fight encountered with %d opponents", fight->beastCount);
    f->fight = fight;
    return fight;
}

void checkRemoveFight(FightManager *f) {
    if (isFightDone(f->fight)) {
        Player *p = f->fight->player;
        for (int i = 0; i < p->partyCount; i++) {
            p->party[i]->actionGauge = 0;
        }
        f->fight = NULL;
        free(f->fight);
    }
}

void resetAfterAttackAction(FightManager *fm, int playerIndex) {
    fm->fight->player->party[playerIndex]->actionGauge = 0;
    Menu *current = getCurrentMenu(fm->menus);
    normalizeMenuCursor(current, fm->ui->menuContext);
    removeMenu(fm->menus);
    removeMenu(fm->menus);
    Menu *newCurrent = getCurrentMenu(fm->menus);
    fm->ui->menuContext->cursorLine = newCurrent->cursor;
    newCurrent->cursor = newCurrent->getNextOption(fm->ui->menuContext);
    normalizeMenuCursor(newCurrent, fm->ui->menuContext);
}

void resetAfterSpellAction(FightManager *fm, int playerIndex) {
    fm->fight->player->party[playerIndex]->actionGauge = 0;
    Menu *c = getCurrentMenu(fm->menus);
    normalizeMenuCursor(c, fm->ui->menuContext);
    removeMenu(fm->menus);
    getCurrentMenu(fm->menus)->cursor = 0;
    removeMenu(fm->menus);
    getCurrentMenu(fm->menus)->cursor = 0;
    removeMenu(fm->menus);
    Menu *newCurrent = getCurrentMenu(fm->menus);
    newCurrent->cursor = newCurrent->getNextOption(fm->ui->menuContext);
    normalizeMenuCursor(newCurrent, fm->ui->menuContext);
}

void destroyBeast(FightManager *fm, int index) {
    free(fm->fight->beasts[index]);
    for (int i = index; i < fm->fight->beastCount - 1; i++) {
        fm->fight->beasts[i] = fm->fight->beasts[i + 1];
    }
    fm->fight->beasts[fm->fight->beastCount] = NULL;
    fm->fight->beastCount--;
}

void attackBeast(FightManager *fm, int targetIndex) {
    Menu *m = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    Mobile *mob = fm->fight->player->party[m->cursor];
    Beast *beast = fm->fight->beasts[targetIndex];
    beast->hp -= calculateAttributes(mob).strength;
    if (beast->hp < 0) {
        destroyBeast(fm, targetIndex);
    }
}

void attackMobile(FightManager *fm, int targetIndex) {
    Menu *m = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    Mobile *target = fm->fight->player->party[targetIndex];
    Mobile *attacker = fm->fight->player->party[m->cursor];
    target->hp -= calculateAttributes(attacker).strength;
    resetAfterAttackAction(fm, m->cursor);
    fm->ui->menuContext->cursorLine = m->cursor;
    m->cursor = m->getNextOption(fm->ui->menuContext);
}

void applyCastCost(Mobile *caster, Attributes cost) {
    caster->mana -= cost.mana;
}

void castBeast(FightManager *fm) {
    Menu *targetMenu = findMenu(fm->menus, BEAST_TARGET_FIGHT_MENU);
    Menu *spellMenu = findMenu(fm->menus, MAGIC_FIGHT_MENU);
    Menu *casterMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    Beast *target = fm->fight->beasts[targetMenu->cursor];
    Mobile *caster = fm->fight->player->party[casterMenu->cursor];
    applyCastCost(caster, caster->spells[spellMenu->cursor]->cost);
    // todo cast
    if (target->hp < 0) {
        destroyBeast(fm, targetMenu->cursor);
    }
}

void castMobile(FightManager *fm) {
    Menu *targetMenu = findMenu(fm->menus, MOBILE_TARGET_FIGHT_MENU);
    Menu *spellMenu = findMenu(fm->menus, MAGIC_FIGHT_MENU);
    Menu *casterMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
//    Beast *target = fm->fight->player->party[targetMenu->cursor];
    Mobile *caster = fm->fight->player->party[casterMenu->cursor];
    applyCastCost(caster, caster->spells[spellMenu->cursor]->cost);
    // todo cast
    fm->ui->menuContext->cursorLine = targetMenu->cursor;
    targetMenu->cursor = targetMenu->getNextOption(fm->ui->menuContext);
}

void fightSpaceKeyPressed(FightManager *fm) {
    Menu *currentMenu = getCurrentMenu(fm->menus);
    int c = currentMenu->cursor;
    if (c > -1) {
        MenuSelectResponse *response = menuItemSelected(
                fm->menus,
                fm->ui->menus,
                fm->ui->menuContext);
        if (response->type == FIND_TARGET_MENU) {
            Menu *previous = getPreviousMenu(fm->menus);
            if (previous->type == MAGIC_FIGHT_MENU) {
                if (currentMenu->type == BEAST_TARGET_FIGHT_MENU) {
                    castBeast(fm);
                } else {
                    castMobile(fm);
                }
                Menu *mobileSelectMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
                resetAfterSpellAction(fm, mobileSelectMenu->cursor);
            } else {
                if (currentMenu->type == BEAST_TARGET_FIGHT_MENU) {
                    attackBeast(fm, currentMenu->cursor);
                } else {
                    attackMobile(fm, currentMenu->cursor);
                }
                Menu *mobileSelectMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
                resetAfterAttackAction(fm, mobileSelectMenu->cursor);
            }
        }
        free(response);
    }
}

void tryToggleTargetMenus(FightManager *fm) {
    Menu *current = getCurrentMenu(fm->menus);
    if (current->type == BEAST_TARGET_FIGHT_MENU) {
        removeMenu(fm->menus);
        Menu *m = findMenu(fm->ui->menus, MOBILE_TARGET_FIGHT_MENU);
        Mobile *mob = fm->fight->player->party[m->cursor];
        if (mob->hp <= 0) {
            fm->ui->menuContext->cursorLine = m->cursor;
            m->cursor = m->getNextOption(fm->ui->menuContext);
        }
        addMenu(fm->menus, m);
    } else if (current->type == MOBILE_TARGET_FIGHT_MENU) {
        removeMenu(fm->menus);
        addMenu(fm->menus, findMenu(fm->ui->menus, BEAST_TARGET_FIGHT_MENU));
    }
}

void checkFightInput(FightManager *fm) {
    addDebug(fm->log, "fight -- check player input");
    if (IsKeyPressed(KEY_ESCAPE)) {
        Menu *m = getCurrentMenu(fm->menus);
        if (m->type == MOBILE_SELECT_FIGHT_MENU) {
            return;
        }
        removeMenu(fm->menus);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Menu *m = getCurrentMenu(fm->menus);
        MenuContext *mc = fm->ui->menuContext;
        m->cursor = m->getNextOption(mc);
        normalizeMenuCursor(m, mc);
    }
    if (IsKeyPressed(KEY_UP)) {
        Menu *m = getCurrentMenu(fm->menus);
        MenuContext *mc = fm->ui->menuContext;
        m->cursor = m->getPreviousOption(mc);
        normalizeMenuCursor(m, mc);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        fightSpaceKeyPressed(fm);
    }
    if (IsKeyPressed(KEY_LEFT)) {
        tryToggleTargetMenus(fm);
    }
    if (IsKeyPressed(KEY_RIGHT)) {
        tryToggleTargetMenus(fm);
    }
}
