#include <raylib.h>
#include <assert.h>
#include "headers/util/log.h"
#include "headers/player.h"
#include "headers/graphics/ui/menu.h"
#include "headers/notification.h"
#include "headers/fight.h"
#include "headers/util/util.h"
#include "headers/action.h"
#include "headers/mobile.h"
#include "headers/spell_manager.h"
#include "headers/ui_manager.h"

typedef struct {
    Context *context;
    UIManager *ui;
    Fight *fight;
    Menu **menus;
    SpellManager *spells;
    NotificationManager *notifications;
} FightManager;

FightManager *createFightManager(
        Context *c,
        UIManager *uiMan,
        SpellManager *spells,
        NotificationManager *notifications) {
    FightManager *f = malloc(sizeof(FightManager));
    f->context = c;
    f->ui = uiMan;
    f->spells = spells;
    f->fight = NULL;
    f->menus = calloc(MAX_MENUS, sizeof(Menu));
    addMenu(f->menus, findMenu(uiMan->menus, BEAST_LIST_FIGHT_MENU));
    addMenu(f->menus, findMenu(uiMan->menus, MOBILE_SELECT_FIGHT_MENU));
    f->notifications = notifications;
    return f;
}

bool isFighting(const FightManager *f) {
    return f->fight != NULL;
}

Fight *createFight(
        FightManager *fm,
        Beast **beasts,
        Player *player,
        int beastCount) {
    Fight *fight = malloc(sizeof(Fight));
    fight->context = fm->context;
    if (beastCount < 1) {
        addError("beast count was less than 1: %d", beastCount);
        exit(GameEngineErrorZeroBeastsToCreateForFight);
    }
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
        fight->mobsDefending[i] = false;
    }
    fight->menu = 0;
    fight->actionCount = 0;
    fight->cancelled = false;
    fight->cancelledAt = 0;
    fm->fight = fight;
    return fight;
}

Fight *createFightFromEncounters(
        FightManager *f,
        const Encounters *encounters,
        Player *player) {
    int maxBeasts = player->partyCount * 2;
    if (maxBeasts > MAX_BEASTS_IN_FIGHT) {
        maxBeasts = MAX_BEASTS_IN_FIGHT;
    }
    Beast *beasts[maxBeasts];
    int beastsToCreate = GetRandomValue(1, maxBeasts);
    addDebug("creating %d beasts for fight", beastsToCreate);
    int created = 0;
    float x = BEAST_AREA.x;
    float y = BEAST_AREA.y;
    while (created < beastsToCreate) {
        int e = GetRandomValue(0, encounters->beastEncountersCount - 1);
        int max = encounters->beastEncounters[e]->max;
        int amount = GetRandomValue(0, max);
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
    Fight *fight = createFight(f, beasts, player, created);
    addDebug("fight encountered with %d opponents", fight->beastCount);
    return fight;
}

void checkRemoveFight(FightManager *f) {
    if (isFightDone(f->fight)) {
        const Player *p = f->fight->player;
        for (int i = 0; i < p->partyCount; i++) {
            p->party[i]->actionGauge = 0;
            p->party[i]->isFleeing = false;
            p->party[i]->step = STEP_NONE;
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
    if (!found) {
        addWarning("beast not found");
        return;
    }
    fm->fight->beasts[fm->fight->beastCount] = NULL;
    fm->fight->beastCount--;
    free(beast);
}

Mobile *getFinalMobileTarget(const Fight *fight, Mobile *mob) {
    int index = -1;
    for (int i = 0; i < fight->player->partyCount; i++) {
        if (fight->player->party[i] == mob) {
            index = i;
            if (mob->hp > 0) {
                return mob;
            }
        }
    }
    for (int i = index; i < fight->player->partyCount; i++) {
        if (fight->player->party[i]->hp > 0) {
            return fight->player->party[i];
        }
    }
    for (int i = 0; i < index; i++) {
        if (fight->player->party[i]->hp > 0) {
            return fight->player->party[i];
        }
    }
    return NULL;
}

Beast *getFinalBeastTarget(const Fight *fight, ActionParticipant *ap) {
    assert(fight->beasts != NULL);
    for (int i = 0; i < fight->beastCount; i++) {
        if (fight->beasts[i] == ap->beast) {
            return ap->beast;
        }
    }
    for (int i = ap->beastIndex; i < fight->beastCount; i++) {
        if (fight->beasts[i] != NULL) {
            return fight->beasts[i];
        }
    }
    for (int i = 0; i < ap->beastIndex; i++) {
        if (fight->beasts[i]->hp > 0) {
            return fight->beasts[i];
        }
    }
    return NULL;
}

void attackBeast(FightManager *fm, Action *act) {
    Beast *beast = getFinalBeastTarget(fm->fight, act->target);
    assert(beast != NULL);
    beast->hp -= act->initiator->mob != NULL
                 ? calculateMobileAttributes(act->initiator->mob).strength
                 : calculateBeastAttributes(act->initiator->beast).strength;
    beast->hitAnimationTimer = HIT_ANIMATION_TIMER_MS;
    if (beast->hp < 0) {
        destroyBeast(fm, beast);
    }
}

void attackMobile(const FightManager *fm, Action *act) {
    Mobile *mob = getFinalMobileTarget(fm->fight, act->target->mob);
    if (!mob) {
        return;
    }
    mob->hp -= act->initiator->mob != NULL
               ? calculateMobileAttributes(act->initiator->mob).strength
               : calculateBeastAttributes(act->initiator->beast).strength;
    normalizeVitalsForMobile(mob);
    mob->hitAnimationTimer = HIT_ANIMATION_TIMER_MS;
}

void castOnBeast(FightManager *fm, Action *act) {
    Beast *target = act->target->beast;
    Mobile *caster = act->initiator->mob;
    const Spell *spell = act->object->spell;
    if (!applyCastCost(caster, spell->cost)) {
        return;
    }
    executeSpellOnBeast(target, spell);
    if (target->hp < 0) {
        destroyBeast(fm, target);
    }
}

void castOnMobile(FightManager *fm) {
    Mobile *target = fm->ui->menuContext->targetMob;
    Mobile *caster = fm->ui->menuContext->selectedMob;
    const Spell *spell = fm->ui->menuContext->selectedSpell;
    if (!applyCastCost(caster, spell->cost)) {
        return;
    }
    executeSpellOnMobile(target, spell);
}

void startDefending(MenuContext *mc) {
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (mc->selectedMob == mc->player->party[i]) {
            mc->fight->mobsDefending[i] = true;
            mc->selectedMob->actionGauge = 0;
            return;
        }
    }
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

void applyConsumableToBeast(Beast *beast, const Item *item) {
    beast->hp += item->attributes->hp;
    beast->mana += item->attributes->mana;
}

void consumeItem(FightManager *fm, Action *act) {
    if (act->target->mob != NULL) {
        applyConsumable(act->target->mob, act->object->item);
    } else {
        applyConsumableToBeast(act->target->beast, act->object->item);
    }
    removeItem(fm->fight->player, act->object->item);
}

void queueMobFightAction(FightManager *fm) {
    const Menu *mobileSelectMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    MenuContext *mc = fm->ui->menuContext;
    ActionObject *o = NULL;
    if (mc->actionType == CAST) {
        o = createActionSpellObject(mc->selectedSpell);
    } else if (fm->ui->menuContext->actionType == ITEM) {
        o = createActionItemObject(mc->selectedItem);
    }
    int beastIndex = -1;
    if (mc->targetBeast != NULL) {
        for (int i = 0; i < fm->fight->beastCount; i++) {
            if (mc->targetBeast == fm->fight->beasts[i]) {
                beastIndex = i;
                break;
            }
        }
    }
    addAction(
            fm->fight,
            createAction(
                    mc->actionType,
                    ATTACK_QUEUE,
                    createMobParticipant(fm->fight->player->party[mobileSelectMenu->cursor]),
                    mc->targetBeast != NULL
                    ? createBeastParticipant(mc->targetBeast, beastIndex)
                    : createMobParticipant(mc->targetMob),
                    o));
    fm->fight->mobsDefending[mobileSelectMenu->cursor] = false;
}

void queueBeastFightAction(FightManager *fm, Beast *beast) {
    ActionObject *o = NULL;
    int index = -1;
    for (int i = 0; i < fm->fight->beastCount; i++) {
        if (fm->fight->beasts[i] == beast) {
            index = i;
        }
    }
    int i = GetRandomValue(0, fm->fight->player->partyCount - 1);
    addAction(
            fm->fight,
            createAction(
                    ATTACK,
                    ATTACK_STEP_OUT,
                    createBeastParticipant(beast, index),
                    createMobParticipant(fm->fight->player->party[i]),
                    o));
    beast->step = ATTACK_QUEUE;
    fm->fight->beastsDefending[i] = false;
}

void attemptToRun(FightManager *fm) {
    const Menu *mobileSelectMenu = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    Mobile *mob = fm->fight->player->party[mobileSelectMenu->cursor];
    ActionParticipant *participant = createMobParticipant(mob);
    mob->isFleeing = true;
    addAction(
            fm->fight,
            createAction(
                    RUN,
                    ACTION_STEP_RUN,
                    participant,
                    participant,
                    NULL));
}

int getActionGaugeRaise(double elapsedTime, int dexterity) {
    int amount = (int) (elapsedTime + dexterity) / 10;
    return amount > MAX_ACTION_GAUGE_RAISE ? MAX_ACTION_GAUGE_RAISE : amount;
}

int normalizeActionGauge(int current, int amount) {
    int total = current + amount;
    return total > MAX_ACTION_GAUGE ? MAX_ACTION_GAUGE : total;
}

void attackAction(FightManager *fm, Action *act) {
    if (act->type == ATTACK) {
        attack(fm, act);
    } else if (act->type == CAST) {
        castSpell(fm, act);
    } else if (act->type == ITEM) {
        consumeItem(fm, act);
    } else {
        return;
    }
}

void clearAction(FightManager *fm, Action *a) {
    setParticipantStep(a->initiator, STEP_NONE); // shouldn't be necessary
    finishAction(a);
    removeAction(fm->fight);
    free(a);
}

bool tryFlee(FightManager *fm, Action *act) {
    bool option = (bool) GetRandomValue(0, 1);
    if (option) {
        addNotification(
                fm->notifications,
                createNotification(FIGHT_ACTION, "You successfully ran away."));
        cancelFight(fm->fight);
    } else {
        if (act->initiator->mob != NULL) {
            act->initiator->mob->isFleeing = false;
        }
        addNotification(
                fm->notifications,
                createNotification(FIGHT_ACTION, "You failed to run away!"));
    }
    return option;
}

void actionUpdate(FightManager *fm, double interval) {
    Action *act = fm->fight->actions[0];
    act->elapsedTime += interval;
    ActionStep step = getParticipantActionStep(act->initiator);
    if (step == ATTACK_QUEUE) {
        step = ATTACK_STEP_OUT;
    } else if (step == ATTACK_STEP_OUT && act->elapsedTime > STEP_OUT_TIMEOUT_MS) {
        step = ATTACK_ACTION;
    } else if (step == ATTACK_ACTION && act->elapsedTime > ACTION_TIMEOUT_MS) {
        attackAction(fm, act);
        step = ATTACK_RETURN;
    } else if (step == ATTACK_RETURN && act->elapsedTime > RETURN_TIMEOUT_MS) {
        step = STEP_NONE;
    } else if (step == ACTION_STEP_RUN && act->elapsedTime > RUN_TIMEOUT_MS) {
        tryFlee(fm, act);
        step = STEP_NONE;
    } else {
        return;
    }
    setParticipantStep(act->initiator, step);
    act->elapsedTime = 0;
    if (step == STEP_NONE) {
        clearAction(fm, act);
    }
}

void raiseBeastsActionGauge(FightManager *fm, double interval) {
    for (int i = 0; i < fm->fight->beastCount; i++) {
        Beast *b = fm->fight->beasts[i];
        int amountToRaise = getActionGaugeRaise(interval, b->attributes->dexterity);
        if (b->actionGauge < MAX_ACTION_GAUGE) {
            b->actionGauge = normalizeActionGauge(b->actionGauge, amountToRaise);
        } else if (b->step == STEP_NONE) {
            queueBeastFightAction(fm, b);
        }
    }
}

void raiseMobsActionGauge(FightManager *fm, double interval) {
    for (int i = 0; i < fm->fight->player->partyCount; i++) {
        Mobile *mob = fm->fight->player->party[i];
        int amountToRaise = getActionGaugeRaise(interval, calculateMobileAttributes(mob).dexterity);
        if (!isReadyForAction(mob) && mob->hp > 0) {
            mob->actionGauge = normalizeActionGauge(mob->actionGauge, amountToRaise);
        }
    }
}

void ensureActiveCursor(FightManager *fm) {
    Menu *m = findMenu(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    Mobile *currentMob = fm->fight->player->party[m->cursor];
    if (currentMob == NULL || !isReadyForAction(currentMob)) {
        for (int i = 0; i < fm->fight->player->partyCount; i++) {
            if (isReadyForAction(fm->fight->player->party[i])) {
                m->cursor = i;
            }
        }
    }
}

void reduceHitAnimationTimer(const FightManager *fm, double interval) {
    for (int i = 0; i < fm->fight->beastCount; i++) {
        if (fm->fight->beasts[i]->hitAnimationTimer > 0) {
            fm->fight->beasts[i]->hitAnimationTimer -= (float) interval;
        }
    }
    for (int i = 0; i < fm->fight->player->partyCount; i++) {
        if (fm->fight->player->party[i]->hitAnimationTimer > 0) {
            fm->fight->player->party[i]->hitAnimationTimer -= (float) interval;
        }
    }
}

void fightUpdate(FightManager *fm) {
    Fight *fight = fm->fight;
    if (fight->cancelled) {
        return;
    }
    double end = getTimeInMS();
    double interval = end - fight->time;
    raiseBeastsActionGauge(fm, interval);
    raiseMobsActionGauge(fm, interval);
    if (fight->actionCount > 0) {
        actionUpdate(fm, interval);
    }
    reduceHitAnimationTimer(fm, interval);
    ensureActiveCursor(fm);
    fight->time = end;
}

void removeMenusUntilMobileSelect(FightManager *fm) {
    const Menu *currentMenu = getCurrentMenu(fm->menus);
    Menu *menu = removeMenusTo(fm->menus, MOBILE_SELECT_FIGHT_MENU);
    MenuContext *mc = fm->ui->menuContext;
    mc->cursorLine = menu->cursor;
    menu->cursor = currentMenu->getNextOption(mc, currentMenu->getCursorLength(mc));
}

void fightSpaceKeyPressed(FightManager *fm) {
    const Menu *currentMenu = getCurrentMenu(fm->menus);
    int c = currentMenu->cursor;
    if (c > -1) {
        MenuSelectResponse *response = menuItemSelected(
                fm->menus,
                fm->ui->menus,
                fm->ui->menuContext);
        if (response->type == RESPONSE_TYPE_FIND_TARGET_MENU) {
            queueMobFightAction(fm);
            removeMenusUntilMobileSelect(fm);
        } else if (response->type == RESPONSE_TYPE_DEFEND_SELECTED) {
            startDefending(fm->ui->menuContext);
            removeMenusUntilMobileSelect(fm);
        } else if (response->type == RESPONSE_TYPE_RUN) {
            attemptToRun(fm);
            removeMenusUntilMobileSelect(fm);

        }
        free(response);
    }
}

void tryToggleTargetMenus(FightManager *fm) {
    const Menu *current = getCurrentMenu(fm->menus);
    if (current->type == BEAST_TARGET_FIGHT_MENU) {
        MenuContext *mc = fm->ui->menuContext;
        removeLastMenu(fm->menus, mc);
        Menu *m = findMenu(fm->ui->menus, MOBILE_TARGET_FIGHT_MENU);
        const Mobile *mob = fm->fight->player->party[m->cursor];
        if (mob->hp <= 0) {
            mc->cursorLine = m->cursor;
            m->cursor = m->getNextOption(mc, m->getCursorLength(mc));
        }
        addMenu(fm->menus, m);
    } else if (current->type == MOBILE_TARGET_FIGHT_MENU) {
        removeLastMenu(fm->menus, fm->ui->menuContext);
        addMenu(fm->menus, findMenu(fm->ui->menus, BEAST_TARGET_FIGHT_MENU));
    }
}

void checkFightInput(FightManager *fm) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        const Menu *m = getCurrentMenu(fm->menus);
        if (m->type == MOBILE_SELECT_FIGHT_MENU) {
            return;
        }
        removeLastMenu(fm->menus, fm->ui->menuContext);
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Menu *m = getCurrentMenu(fm->menus);
        const MenuContext *mc = fm->ui->menuContext;
        m->cursor = m->getNextOption(mc, m->getCursorLength(mc));
    }
    if (IsKeyPressed(KEY_UP)) {
        Menu *m = getCurrentMenu(fm->menus);
        const MenuContext *mc = fm->ui->menuContext;
        m->cursor = m->getPreviousOption(mc, m->getCursorLength(mc));
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
