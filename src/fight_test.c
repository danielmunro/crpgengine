#include <tap.h>
#include "headers/fight.h"
#include "headers/util/test_util.h"
#include "headers/persistence/loader.h"
#include "headers/player_manager.h"
#include "headers/fight_manager.h"

typedef struct {
    Encounters *e;
    FightManager *fm;
    MobileManager *mm;
    ItemManager *im;
} FightInScene;

Beast *createTestBeast() {
    Beast *beast = malloc(sizeof(Beast));
    beast->id = 0;
    beast->name = "foo";
    beast->gp = 1;
    beast->hp = 20;
    beast->mana = 100;
    beast->level = 100;
    beast->attributes = createEmptyAttributes();
    return beast;
}

FightInScene *createFightInScene(Context *c) {
    FightInScene *fightInScene = malloc(sizeof(FightInScene));
    fightInScene->e = createEncounters();
    fightInScene->e->beastEncountersCount = 1;
    fightInScene->e->beastEncounters[0] = createBeastEncounter(createTestBeast(), 5);
    UIManager *uiMan = createUIManager(
            loadUIData(c->indexDir),
            NULL,
            NULL,
            NULL);
    SpellManager *sm = loadSpellManager(c->indexDir);
    fightInScene->fm = createFightManager(c, uiMan, sm, NULL);
    fightInScene->im = createItemManager(c);
    loadAllItems(fightInScene->im);
    fightInScene->mm = createMobileManager(
            sm,
            createTestAnimationManager(c),
            c);
    loadPlayerMobiles(fightInScene->mm);
    return fightInScene;
}

Fight *createFightFromFightInScene(FightInScene *fightInScene) {
    return createFightFromEncounters(
            fightInScene->fm,
            fightInScene->e,
            createNewPlayer(fightInScene->mm, fightInScene->im));
}

void createFightInSceneTest(Context *c) {
    FightInScene *fightInScene = createFightInScene(c);
    for (int i = 0; i < 100; i++) {
        createFightFromFightInScene(fightInScene);
        const Fight *f = fightInScene->fm->fight;
        char message[MAX_LOG_LINE_LENGTH];
        sprintf(message, "beast count is within expected range: %d", f->beastCount);
        ok(0 < f->beastCount && f->beastCount <= 9, message);
    }
    free(fightInScene);
}