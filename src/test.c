#include <tap.h>
#include "headers/test.h"
#include "headers/global_setup.h"

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

AnimationManager *createTestAnimationManager(Context *c) {
    AnimationManager *am = createAnimationManager(c);
    SpritesheetManager *sm = loadSpritesheetManager(c);
    loadAllAnimations(am, sm);
    free(sm);
    return am;
}

Attributes *createTestAttributes() {
    Attributes *a = createEmptyAttributes();
    a->hp = STARTING_HP;
    a->mana = STARTING_MANA;
    a->strength = 5;
    return a;
}

typedef struct {
    Encounters *e;
    FightManager *fm;
    MobileManager *mm;
    ItemManager *im;
} FightInScene;

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
}

void canMoveMobTest(Context *c) {
    // given
    float startX = 100;
    float startY = 100;
    Vector2 start = (Vector2) {startX, startY};
    AnimationManager *am = createTestAnimationManager(c);
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);

    Mobile *mob = createMobile(
            "test",
            "test",
            start,
            DIRECTION_DOWN,
            animations,
            createAvatar(
                    c->indexDir,
                    "fireas.png",
                    c->ui->screen->scale),
            20,
            20,
            createTestAttributes(),
            mapDataToSpells(NULL, NULL, 0),
            0,
            false);

    // when
    moveMob(
            mob,
            (Vector2) {startX + 5, startY + 5},
            (float) c->ui->screen->targetFrameRate);

    // then
    ok(mob->position.x > startX && mob->position.y > startY, "mob moved as expected");
    ok(mob->direction == DIRECTION_RIGHT, "mob is facing right");
}

void canMobStopMovingTest(Context *c) {
    // given
    float startX = 100;
    float startY = 100;
    Vector2 start = (Vector2) {startX, startY};
    AnimationManager *am = createTestAnimationManager(c);
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    Mobile *mob = createMobile(
            "test",
            "test",
            start,
            DIRECTION_DOWN,
            animations,
            createAvatar(
                    c->indexDir,
                    "fireas.png",
                    c->ui->screen->scale),
            STARTING_HP,
            STARTING_MANA,
            createTestAttributes(),
            mapDataToSpells(NULL, NULL, 0),
            0,
            false);

    // when
    moveMob(mob, start, (float) c->ui->screen->targetFrameRate);

    // then
    ok(mob->position.x == startX && mob->position.y == startY, "mob moved as expected");
    ok(!isMoving(mob), "mob is not moving");
}

void experienceToLevel1Test() {
    // when
    int experience = getExperienceToLevel(1);

    // then
    ok(experience == 1000, "experience value was expected :: %d", experience);
}

void experienceToLevel51Test() {
    // when
    int experience = getExperienceToLevel(51);

    // then
    ok(experience == 133650, "experience value was expected :: %d", experience);
}

int main() {
    Context *c = globalSetup();
    plan(106);
    createFightInSceneTest(c);
    canMoveMobTest(c);
    canMobStopMovingTest(c);
    experienceToLevel1Test();
    experienceToLevel51Test();
    done_testing();
}