typedef struct {
    UIManager *ui;
    Fight *fight;
    Menu **menus;
    SpellManager *spells;
} FightManager;

FightManager *createFightManager(UIManager *uiMan, SpellManager *spells) {
    FightManager *f = malloc(sizeof(FightManager));
    f->ui = uiMan;
    f->spells = spells;
    f->fight = NULL;
    f->menus = calloc(MAX_MENUS, sizeof(Menu));
    addMenu(f->menus, findMenu(uiMan->menus, BEAST_LIST_FIGHT_MENU));
    addMenu(f->menus, findMenu(uiMan->menus, MOBILE_SELECT_FIGHT_MENU));
    return f;
}

bool isFighting(FightManager *f) {
    return f->fight != NULL;
}

Fight *createFightFromEncounters(
        FightManager *f,
        Encounters *encounters,
        Player *player) {
    Beast *beasts[MAX_BEASTS_IN_FIGHT];
    int beastsToCreate = rand() % MAX_BEASTS_IN_FIGHT + 1;
    addDebug("creating %d beasts for fight", beastsToCreate);
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
    Fight *fight = createFight(beasts, player, created);
    fight->beastCount = created;
    addDebug("fight encountered with %d opponents", fight->beastCount);
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

void destroyBeast(FightManager *fm, Beast *beast) {
    int found = false;
    for (int i = 0; i < fm->fight->beastCount; i++) {
        if (fm->fight->beasts[i] == beast) {
            found = true;
        }
        if (found) {
            fm->fight->beasts[i] = fm->fight->beasts[i + 1];
        }
    }
    fm->fight->beasts[fm->fight->beastCount] = NULL;
    fm->fight->beastCount--;
}

void attackBeast(FightManager *fm, Action *act) {
    Beast *beast = act->target->beast;
    beast->hp -= calculateMobileAttributes(act->initiator->mob).strength;
    if (beast->hp < 0) {
        destroyBeast(fm, beast);
    }
}

void attackMobile(FightManager *fm, Action *act) {
    Menu *m = findMenu(fm->menus, MOBILE_TARGET_FIGHT_MENU);
    Mobile *attacker = fm->fight->player->party[m->cursor];
    act->target->mob->hp -= calculateMobileAttributes(attacker).strength;
}

int getAttributeAmount(Spell *spell, int base) {
    if (base == 0) {
        return 0;
    }
    int amount = base + (spell->level * (int) spell->levelModifier);
    if (spell->intent == INTENT_HARM) {
        return -amount;
    } else if (spell->intent == INTENT_HELP) {
        return amount;
    } else {
        fprintf(stderr, "unknown intent in getAttributeAmount");
        exit(EXIT_UNKNOWN_INTENT);
    }
}

void executeSpellOnBeast(Beast *beast, Spell *spell) {
    beast->hp += (int) getAttributeAmount(spell, spell->impact->hp);
    beast->mana += (int) getAttributeAmount(spell, spell->impact->mana);
    normalizeVitalsForBeast(beast);
}

void executeSpellOnMobile(Mobile *mob, Spell *spell) {
    mob->hp += (int) getAttributeAmount(spell, spell->impact->hp);
    mob->mana += (int) getAttributeAmount(spell, spell->impact->mana);
    normalizeVitalsForMobile(mob);
}

void castOnBeast(FightManager *fm, Action *act) {
    Beast *target = act->target->beast;
    Mobile *caster = act->initiator->mob;
    Spell *spell = act->object->spell;
    if (!applyCastCost(caster, spell->cost)) {
        return;
    }
    executeSpellOnBeast(target, spell);
    if (target->hp < 0) {
        destroyBeast(fm, target);
    }
}

void castOnMobile(FightManager *fm) {
    Menu *targetMenu = findMenu(fm->menus, MOBILE_TARGET_FIGHT_MENU);
    Menu *spellMenu = findMenu(fm->menus, MAGIC_FIGHT_MENU);
    Menu *casterMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    Mobile *target = fm->fight->player->party[targetMenu->cursor];
    Mobile *caster = fm->fight->player->party[casterMenu->cursor];
    Spell *spell = caster->spells[spellMenu->cursor];
    if (!applyCastCost(caster, spell->cost)) {
        return;
    }
    executeSpellOnMobile(target, spell);
}

void startDefending(Menu **menus, Menu *currentMenu, MenuContext *mc) {
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (mc->selectedMob == mc->player->party[i]) {
            mc->fight->defending[i] = true;
            mc->selectedMob->actionGauge = 0;
            break;
        }
    }
    removeMenu(menus);
    Menu *newCurrent = getCurrentMenu(menus);
    mc->cursorLine = currentMenu->cursor;
    newCurrent->cursor = newCurrent->getNextOption(mc);
}

void castSpell(FightManager *fm, Action *act) {
    if (act->target->beast != NULL) {
        castOnBeast(fm, act);
    } else {
        castOnMobile(fm);
    }
}

void attack(FightManager *fm, Action *act) {
    if (act->target->beast != NULL) {
        attackBeast(fm, act);
    } else {
        attackMobile(fm, act);
    }
}

void fightAction(FightManager *fm, Menu *currentMenu) {
    Menu *mobileSelectMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    addAction(
            fm->fight,
            createAction(
                    ATTACK,
                    ATTACK_STEP_OUT,
                    createMobParticipant(fm->fight->player->party[mobileSelectMenu->cursor]),
                    createBeastParticipant(fm->fight->beasts[currentMenu->cursor]),
                    NULL));
    fm->fight->defending[mobileSelectMenu->cursor] = false;

}

int getActionGaugeRaise(double elapsedTime, int dexterity) {
    int amount = (int) (elapsedTime + dexterity) / 10;
    return amount > MAX_ACTION_GAUGE_RAISE ? MAX_ACTION_GAUGE_RAISE : amount;
}

int normalizeActionGauge(int current, int amount) {
    int total = current + amount;
    return total > MAX_ACTION_GAUGE ? MAX_ACTION_GAUGE : total;
}

void actionUpdate(FightManager *fm, double interval) {
    Action *act = fm->fight->actions[0];
    act->elapsedTime += interval;
    if (act->type == ATTACK) {
        Mobile *mob = act->initiator->mob;
        if (mob != NULL) {
            if (mob->step == ATTACK_QUEUE) {
                mob->step = ATTACK_STEP_OUT;
            } else if (mob->step == ATTACK_STEP_OUT && act->elapsedTime > 500) {
                mob->step = ATTACK_ACTION;
                act->elapsedTime = 0;
            } else if (mob->step == ATTACK_ACTION && act->elapsedTime > 300) {
                if (act->type == ATTACK) {
                    attack(fm, act);
                    mob->actionGauge = 0;
                } else {
                    castSpell(fm, act);
                    mob->actionGauge = 0;
                }
                act->initiator->mob->step = ATTACK_RETURN;
                act->elapsedTime = 0;
            } else if (act->initiator->mob->step == ATTACK_RETURN && act->elapsedTime > 500) {
                removeAction(fm->fight);
                act->initiator->mob->step = STEP_NONE;
            }
        }
    }
}

void fightUpdate(FightManager *fm) {
    Fight *fight = fm->fight;
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
        int amountToRaise = getActionGaugeRaise(interval, calculateMobileAttributes(mob).dexterity);
        if (!isReadyForAction(mob) && mob->hp > 0) {
            mob->actionGauge = normalizeActionGauge(mob->actionGauge, amountToRaise);
            Menu *m = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
            Mobile *currentMob = fight->player->party[m->cursor];
            if (isReadyForAction(mob) && (currentMob == NULL || !isReadyForAction(currentMob))) {
                m->cursor = i;
            }
        }
    }
    if (fight->actionCount > 0) {
        actionUpdate(fm, interval);
    }
    fight->time = end;
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
            fightAction(fm, currentMenu);
            Menu *menu = removeMenusTo(fm->menus, MOBILE_SELECT_FIGHT_MENU);
            fm->ui->menuContext->cursorLine = menu->cursor;
            menu->cursor = currentMenu->getNextOption(fm->ui->menuContext);
        } else if (response->type == DEFEND_SELECTED) {
            startDefending(
                    fm->menus,
                    currentMenu,
                    fm->ui->menuContext);
            currentMenu->cursor = currentMenu->getNextOption(fm->ui->menuContext);
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
    addDebug("fight -- check player input");
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
