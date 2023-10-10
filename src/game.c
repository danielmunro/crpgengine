#include <raylib.h>
#include <string.h>
#include "headers/log.h"
#include "headers/scene_manager.h"
#include "headers/exploration.h"
#include "src/graphics/ui/menu.h"
#include "headers/fight_manager.h"
#include "headers/timing.h"
#include "headers/player.h"
#include "headers/mobile_manager.h"
#include "headers/player_manager.h"
#include "headers/loader.h"
#include "headers/item.h"
#include "headers/control_manager.h"
#include "headers/mobile.h"
#include "headers/scene.h"
#include "headers/animation.h"
#include "headers/draw_fight.h"
#include "headers/ui_manager.h"

typedef struct {
    SceneManager *scenes;
    Player *player;
    AnimationManager *animations;
    AudioManager *audio;
    SpritesheetManager *sprites;
    Beastiary *beastiary;
    ItemManager *items;
    Timing *timing;
    NotificationManager *notifications;
    ControlManager *controls;
    MobileManager *mobiles;
    UIManager *ui;
    FightManager *fights;
    Menu **menus;
    SpellManager *spells;
} Game;

void attemptToUseExit(Game *game, Scene *scene, Entrance *entrance) {
    if (entrance == NULL) {
        addWarning("no entrance found for '%s' scene", scene->name);
        return;
    }
    addDebug("entrance %s found at %f, %f, %f, %f",
             entrance->name,
             entrance->area.x,
             entrance->area.y,
             entrance->area.width,
             entrance->area.height
    );
    getPartyLeader(game->player)->position = (Vector2) {
            entrance->area.x,
            entrance->area.y
    };
    setScene(game->scenes, scene, game->player, entrance->name);
}

void evaluateExits(Game *g) {
    addDebug("exploration -- evaluate exits");
    Exploration *e = g->scenes->current->exploration;
    int ex = atExit(e, g->player);
    if (ex > -1) {
        addDebug("player at exit");
        char *sceneName = e->exits[ex]->scene;
        char *entranceName = e->exits[ex]->to;
        for (int i = 0; i < g->scenes->count; i++) {
            if (strcmp(sceneName, g->scenes->scenes[i]->name) == 0) {
                attemptToUseExit(
                        g,
                        g->scenes->scenes[i],
                        findEntrance(g->scenes->scenes[i]->exploration, entranceName)
                );
                return;
            }
        }
        addWarning("warp not found :: %s, %s",
                sceneName, entranceName);
    }
}

void explorationMenuKeyPressed(Game *g) {
    g->ui->menuContext = createMenuContext(
            g->fights->fight,
            g->player,
            g->ui->fonts,
            g->ui->uiSprite,
            g->spells->spells,
            g->scenes->current->name,
            0);
    addMenu(g->menus, findMenu(g->ui->menus, PARTY_MENU));
}

void checkExplorationInput(Game *g) {
    Mobile *mob = getPartyLeader(g->player);
    addDebug("exploration -- check player input");
    resetMoving(mob);
    if (!canPlayerMove(mob)) {
        return;
    }
    explorationCheckMoveKeys(g->player);
    if (IsKeyPressed(KEY_C)) {
        explorationDebugKeyPressed(mob->position);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        explorationSpaceKeyPressed(g->player, g->scenes->current->activeControlBlocks);
    }
    if (IsKeyPressed(KEY_M)) {
        explorationMenuKeyPressed(g);
    }
    if (IsKeyPressed(KEY_T)) {
        addInfo("player play time :: %ds", g->player->secondsPlayed);
    }
    if (IsKeyPressed(KEY_S)) {
        save(g->player, g->scenes->current->name);
    }
}

void checkMenuInput(Game *g) {
    if (IsKeyPressed(KEY_ESCAPE)) {
        int menuCount = removeMenu(g->menus);
        if (menuCount == 0) {
            free(g->ui->menuContext);
            g->ui->menuContext = NULL;
        }
    }
    if (IsKeyPressed(KEY_DOWN)) {
        Menu *menu = getCurrentMenu(g->menus);
        menu->cursor = menu->getNextOption(g->ui->menuContext);
        normalizeMenuCursor(menu, g->ui->menuContext);
    }
    if (IsKeyPressed(KEY_UP)) {
        Menu *menu = getCurrentMenu(g->menus);
        menu->cursor = menu->getPreviousOption(g->ui->menuContext);
        normalizeMenuCursor(menu, g->ui->menuContext);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        MenuSelectResponse *response = menuItemSelected(g->menus, g->ui->menus, g->ui->menuContext);
        free(response);
    }
}

bool isExploring(Game *g) {
    return !isFighting(g->fights) && !getCurrentMenu(g->menus);
}

bool canTriggerFight(Game *g, const Player *p) {
    if (!isDungeon(g->scenes->current) || isFighting(g->fights) || !isMoving(getPartyLeader(p))) {
        return false;
    }
    return true;
}

void checkFights(Game *g, Scene *s) {
    addDebug("exploration -- check for fight");
    if (!canTriggerFight(g, g->player)) {
        return;
    }
    if (GetRandomValue(1, 100) == 1) {
        createFightFromEncounters(
                g->fights,
                s->encounters,
                g->player);
        Animation *animation = findAnimation(getPartyLeader(g->player)->animations, DIRECTION_LEFT);
        animation->currentFrame = animation->firstFrame;
        g->ui->menuContext = createMenuContext(
                g->fights->fight,
                g->player,
                g->ui->fonts,
                g->ui->uiSprite,
                g->spells->spells,
                NULL,
                0);
    }
}

void doExplorationLoop(Game *g) {
    Scene *s = g->scenes->current;
    checkExplorationInput(g);
    drawExplorationView(
            s->exploration,
            g->player,
            g->notifications,
            s->activeControlBlocks,
            g->ui->fonts->default_);
    doMobileMovementUpdates(s->exploration);
    processAnimations(g->animations);
    evaluateMovement(s->exploration, g->player);
    evaluateExits(g);
    checkControls(g->controls);
    checkFights(g, s);
}

void doFightLoop(Game *g) {
    Scene *s = g->scenes->current;
    fightUpdate(g->fights);
    checkFightInput(g->fights);
    drawFightView(s->encounters, g->fights);
    checkControls(g->controls);
    checkRemoveFight(g->fights);
}

void doInGameMenuLoop(Game *g) {
    BeginDrawing();
    drawAllMenus(
            g->ui->menuContext,
            g->menus);
    if (runtimeArgs->showFPS) {
        DrawFPS(FPS_X, FPS_Y);
    }
    EndDrawing();
    checkMenuInput(g);
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        startTiming(g->timing);
        if (isFighting(g->fights)) {
            doFightLoop(g);
        } else if (getCurrentMenu(g->menus) != NULL) {
            doInGameMenuLoop(g);
        } else if (isExploring(g)) {
            doExplorationLoop(g);
        }
        updateMusicStream(g->audio);
        if (g->player->dialog != NULL) {
            updateDialog(g->player->dialog);
        }
        stopTiming(g->timing);
    }
}

SaveData *initializePlayer(Game *g) {
    char saveFilePath[MAX_FS_PATH_LENGTH];
    if (runtimeArgs->saveFile != NULL) {
        sprintf((char *) saveFilePath, "%s/_saves/%s", runtimeArgs->indexDir, runtimeArgs->saveFile);
    } else {
        strcpy(saveFilePath, getAutosaveFile(runtimeArgs->indexDir));
    }
    SaveData *save = NULL;
    if (FileExists(saveFilePath) && !runtimeArgs->forceNewGame) {
        save = loadSaveData(saveFilePath);
        g->player = mapSaveDataToPlayer(g->spells, g->animations, save);
    } else {
        g->player = createNewPlayer(g->mobiles, g->items);
    }
    g->player->saveFiles = getSaveFiles();
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (g->player->party[i] == NULL) {
            break;
        }
        addMobileToManager(g->mobiles, g->player->party[i]);
    }
    return save;
}

void loadAllMobiles(Game *g) {
    g->mobiles = createMobileManager(g->spells, g->animations);
    loadPlayerMobiles(g->mobiles);
}

Game *createGame() {
    Game *g = malloc(sizeof(Game));
    g->sprites = loadSpritesheetManager();
    UIData *uiData = loadUIData();
    g->ui = createUIManager(
            uiData,
            createUISprite(
                    findSpritesheetByName(g->sprites, uiData->sprite->name),
                    uiData));
    g->animations = createAnimationManager();
    loadAllAnimations(g->animations, g->sprites);
    g->audio = loadAudioManager();
    g->beastiary = loadBeastiary();
    g->items = createItemManager();
    loadAllItems(g->items);
    g->spells = loadSpellManager();
    loadAllMobiles(g);
    SaveData *save = initializePlayer(g);
    g->notifications = createNotificationManager();
    g->timing = createTiming(g->notifications, g->player);
    g->controls = createControlManager(
            g->player,
            g->items,
            g->notifications,
            g->mobiles);
    g->scenes = createSceneManager(g->controls, g->animations, g->audio);
    loadScenesFromFiles(g->scenes, g->mobiles, g->beastiary);
    setSceneBasedOnSave(g->scenes, g->player, save);
    addDebug("done creating game object");
    free(save);
    g->menus = calloc(MAX_MENUS, sizeof(Menu));
    g->fights = createFightManager(g->ui, g->spells, g->notifications);
    return g;
}
