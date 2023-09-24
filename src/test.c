#include "header.h"
#include "tap.h"

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

AnimationManager *createTestAnimationManager() {
    AnimationManager *am = createAnimationManager();
    SpritesheetManager *sm = loadSpritesheetManager();
    loadAllAnimations(am, sm);
    return am;
}

typedef struct {
    Encounters *e;
    FightManager *fm;
    MobileManager *mm;
    ItemManager *im;
} FightInScene;

FightInScene *createFightInScene() {
    FightInScene *fightInScene = malloc(sizeof(FightInScene));
    fightInScene->e = createEncounters();
    fightInScene->e->beastEncountersCount = 1;
    fightInScene->e->beastEncounters[0] = createBeastEncounter(createTestBeast(), 5);
    UIManager *uiMan = createUIManager(
            loadUIData(),
            NULL);
    SpellManager *sm = loadSpellManager();
    fightInScene->fm = createFightManager(uiMan, sm, NULL);
    fightInScene->im = createItemManager();
    loadAllItems(fightInScene->im);
    AnimationManager *am = createAnimationManager();
    loadAllAnimations(am, loadSpritesheetManager());
    fightInScene->mm = createMobileManager(
            sm,
            am);
    loadPlayerMobiles(fightInScene->mm);
    return fightInScene;
}

Fight *createFightFromFightInScene(FightInScene *fightInScene) {
    return createFightFromEncounters(
            fightInScene->fm,
            fightInScene->e,
            createNewPlayer(fightInScene->mm, fightInScene->im));
}

void createFightInSceneTest() {
    FightInScene *fightInScene = createFightInScene();
    for (int i = 0; i < 100; i++) {
        createFightFromFightInScene(fightInScene);
        Fight *f = fightInScene->fm->fight;
        char message[MAX_LOG_LINE_LENGTH];
        sprintf(message, "beast count is within expected range: %d", f->beastCount);
        ok(0 < f->beastCount && f->beastCount <= 9, message);
    }
}

void canMoveMobTest() {
    // given
    float startX = 100, startY = 100;
    Vector2 start = (Vector2) {startX, startY};
    AnimationManager *am = createTestAnimationManager();
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    SpellData *spells = calloc(0, sizeof(SpellData));

    Mobile *mob = createMobile(
            "test",
            "test",
            start,
            DOWN,
            animations,
            createAvatar("fireas.png"),
            20,
            20,
            createStartingAttributes(),
            mapDataToSpells(NULL, spells, 0),
            0);

    // when
    moveMob(mob, (Vector2) {startX + 5, startY + 5});

    // then
    ok(mob->position.x > startX && mob->position.y > startY, "mob moved as expected");
    ok(mob->direction == RIGHT, "mob is facing right");
}

void canMobStopMovingTest() {
    // given
    float startX = 100, startY = 100;
    Vector2 start = (Vector2) {startX, startY};
    AnimationManager *am = createTestAnimationManager();
    Animation *animations[MAX_ANIMATIONS];
    loadAnimationsByName(am, "fireas", animations);
    SpellData *spells = calloc(0, sizeof(SpellData));
    Mobile *mob = createMobile(
            "test",
            "test",
            start,
            DOWN,
            animations,
            createAvatar("fireas.png"),
            STARTING_HP,
            STARTING_MANA,
            createStartingAttributes(),
            mapDataToSpells(NULL, spells, 0),
            0);

    // when
    moveMob(mob, start);

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
    globalSetup(5, (char *[]) {"binary", "-i", "fixtures", "-l", "error"});
    plan(106);
    createFightInSceneTest();
    canMoveMobTest();
    canMobStopMovingTest();
    experienceToLevel1Test();
    experienceToLevel51Test();
    done_testing();
}