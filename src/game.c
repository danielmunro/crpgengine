#include <raylib.h>
#include <string.h>
#include "headers/util/log.h"
#include "headers/scene_manager.h"
#include "headers/map.h"
#include "headers/graphics/ui/menu.h"
#include "headers/fight_manager.h"
#include "headers/timing.h"
#include "headers/player.h"
#include "headers/mobile_manager.h"
#include "headers/player_manager.h"
#include "headers/persistence/loader.h"
#include "headers/item.h"
#include "headers/control_manager.h"
#include "headers/mobile.h"
#include "headers/scene.h"
#include "headers/graphics/animation.h"
#include "headers/graphics/draw_fight.h"
#include "headers/ui_manager.h"
#include "headers/save.h"

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
    SaveFiles *saveFiles;
} Game;

void attemptToUseExit(Game *game, Scene *scene, const Entrance *entrance) {
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
    const Map *m = g->scenes->current->map;
    int ex = atExit(m, g->player);
    if (ex < 0) {
        return;
    }
    const char *sceneName = m->exits[ex]->scene;
    const char *entranceName = m->exits[ex]->to;
    for (int i = 0; i < g->scenes->count; i++) {
        if (strcmp(sceneName, g->scenes->scenes[i]->name) == 0) {
            attemptToUseExit(
                    g,
                    g->scenes->scenes[i],
                    findEntrance(g->scenes->scenes[i]->map, entranceName)
            );
            return;
        }
    }
    addWarning("exit not found :: id: %d, scene: %s, entrance: %s",
               m->exits[ex]->id, sceneName, entranceName);
}

void mapMenuKeyPressed(Game *g) {
    addMenu(g->menus, findMenu(g->ui->menus, PARTY_MENU));
}

void evaluateResponse(const Game *g, const Response *r) {
    if (r->actionTaken == ACTION_TAKEN_OPENED_CHEST) {
        const Chest *c = r->chest;
        char *message = malloc(MAX_NOTIFICATION_LENGTH);
        sprintf(message, "You got:\n(%d) %s and %d coins",
                c->iq->quantity, c->iq->item->name, c->coins);
        addNotification(
                g->notifications,
                createNotification(
                        OPENED_CHEST,
                        message));
    } else if (r->actionTaken == ACTION_TAKEN_SHOPPING) {
        if (g->ui->menuContext->shop != NULL) {
            g->ui->menuContext->shop = NULL;
            return;
        }
        const Scene *s = g->scenes->current;
        for (int i = 0; i < s->shopsCount; i++) {
            if (s->shops[i]->id == r->shop->id) {
                g->ui->menuContext->shop = s->shops[i];
                addMenu(g->menus, findMenu(g->ui->menus, SHOP_WELCOME_MENU));
                return;
            }
        }
        addError("shop not found :: %d", r->shop->id);
    }
}

void checkMapInput(Game *g) {
    Mobile *mob = getPartyLeader(g->player);
    resetMoving(mob);
    if (!canPlayerMove(mob)) {
        return;
    }
    mapCheckMoveKeys(g->player);
    if (IsKeyPressed(KEY_C)) {
        mapDebugKeyPressed(mob->position);
    }
    if (IsKeyPressed(KEY_SPACE)) {
        const Response *r = mapSpaceKeyPressed(
                g->scenes->current->map,
                g->player,
                g->scenes->current->activeControlBlocks);
        if (r->actionTaken != ACTION_TAKEN_NONE) {
            evaluateResponse(g, r);
        }
        free((Response *) r);
    }
    if (IsKeyPressed(KEY_M)) {
        mapMenuKeyPressed(g);
    }
    if (IsKeyPressed(KEY_T)) {
        addInfo("player play time :: %ds", g->player->secondsPlayed);
    }
    if (IsKeyPressed(KEY_S)) {
        const SaveFile *s = save(g->player, g->scenes->current->name);
        addSaveFile(g->saveFiles, s);
    }
}

SaveData *initializePlayer(Game *g) {
    char saveFilePath[MAX_FS_PATH_LENGTH];
    if (config->saveFile != NULL) {
        sprintf((char *) saveFilePath, "%s/_saves/%s", config->indexDir, config->saveFile);
    } else {
        strcpy(saveFilePath, getAutosaveFile(config->indexDir));
    }
    SaveData *save = NULL;
    if (FileExists(saveFilePath) && !config->forceNewGame) {
        save = loadSaveData(saveFilePath);
        g->player = mapSaveDataToPlayer(g->spells, g->animations, save);
    } else {
        g->player = createNewPlayer(g->mobiles, g->items);
    }
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (g->player->party[i] == NULL) {
            break;
        }
        addMobileToManager(g->mobiles, g->player->party[i]);
    }
    return save;
}

void checkMenuInput(Game *g) {
    Menu *menu = getCurrentMenu(g->menus);
    MenuContext *mc = g->ui->menuContext;
    mc->menuType = menu->type;
    const MenuKeyPressedType keyPressed = menu->keyPressed(mc);
    if (keyPressed == KEY_PRESSED_CLOSE_MENU) {
        removeLastMenu(g->menus);
        resetMenuContext(mc);
    } else if (keyPressed == KEY_PRESSED_INCREMENT_CURSOR) {
        menu->cursor = menu->getNextOption(mc, menu->getCursorLength(mc));
    } else if (keyPressed == KEY_PRESSED_DECREMENT_CURSOR) {
        menu->cursor = menu->getPreviousOption(mc, menu->getCursorLength(mc));
    } else if (keyPressed == KEY_PRESSED_EXECUTE) {
        MenuSelectResponse *response = menuItemSelected(
                g->menus,
                g->ui->menus,
                mc);
        if (response->type == RESPONSE_TYPE_SAVE_GAME) {
            const SaveFile *s = save(g->player, g->scenes->current->name);
            addSaveFile(g->saveFiles, s);
            addMenu(g->menus, findMenu(g->ui->menus, ACKNOWLEDGE_SAVE_MENU));
        } else if (response->type == RESPONSE_TYPE_LOAD_GAME) {
            config->saveFile = g->saveFiles->saves[mc->cursorLine]->saveName;
            initializePlayer(g);
            removeMenu(g->menus, MAIN_MENU);
        }
        free(response);
    } else if (keyPressed == KEY_PRESSED_INCREMENT_QUANTITY) {
        mc->quantity += 1;
    } else if (keyPressed == KEY_PRESSED_DECREMENT_QUANTITY) {
        mc->quantity -= 1;
    }
}

bool isExploring(Game *g) {
    return !isFighting(g->fights) && !getCurrentMenu(g->menus);
}

bool canTriggerFight(const FightManager *fm, const Scene *s, const Player *p) {
    return isDungeon(s)
            && !isFighting(fm)
            && isMoving(getPartyLeader(p))
            && config->randomEncounters;
}

void checkFights(Game *g, const Scene *s) {
    if (!canTriggerFight(g->fights, s, g->player)) {
        return;
    }
    if (GetRandomValue(1, 100) == 1) {
        createFightFromEncounters(
                g->fights,
                s->encounters,
                g->player);
        Animation *animation = findAnimation(getPartyLeader(g->player)->animations, DIRECTION_LEFT);
        animation->currentFrame = animation->firstFrame;
        g->ui->menuContext->fight = g->fights->fight;
    }
}

void doExplorationLoop(Game *g) {
    Scene *s = g->scenes->current;
    checkMapInput(g);
    drawMapView(
            s->map,
            g->player,
            g->notifications,
            s->activeControlBlocks,
            g->ui->fonts->default_);
    processAnimations(g->animations);
    doMobileMovementUpdates(s->map);
    evaluateMovement(s->map, g->player);
    evaluateExits(g);
    checkControls(g->controls);
    checkFights(g, s);
}

void doFightLoop(Game *g) {
    Scene *s = g->scenes->current;
    fightUpdate(g->fights);
    if (!isFightDone(g->fights->fight)) {
        checkFightInput(g->fights);
        drawFightView(s->encounters, g->fights);
        checkControls(g->controls);
    }
    checkRemoveFight(g->fights);
}

void doInGameMenuLoop(Game *g) {
    BeginDrawing();
    drawAllMenus(
            g->ui->menuContext,
            g->menus);
    if (config->showFPS) {
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

void loadAllMobiles(Game *g) {
    g->mobiles = createMobileManager(g->spells, g->animations);
    loadPlayerMobiles(g->mobiles);
}

void initializeGameForPlayer(Game *g) {
    SaveData *save = initializePlayer(g);
    g->controls = createControlManager(
            g->player,
            g->items,
            g->notifications,
            g->mobiles,
            g->saveFiles);
    g->scenes = createSceneManager(g->controls, g->animations, g->audio);
    loadScenesFromFiles(
            g->scenes,
            g->mobiles,
            g->items,
            g->beastiary);
    setSceneBasedOnSave(g->scenes, g->player, save);
    g->timing->player = g->player;
    g->ui->menuContext->player = g->player;
    addDebug("done initializing game for player");
    free(save);
}

Game *createGame() {
    Game *g = malloc(sizeof(Game));
    g->sprites = loadSpritesheetManager();
    UIData *uiData = loadUIData();
    g->animations = createAnimationManager();
    loadAllAnimations(g->animations, g->sprites);
    g->audio = loadAudioManager();
    g->beastiary = loadBeastiary();
    g->items = createItemManager();
    loadAllItems(g->items);
    g->spells = loadSpellManager();
    loadAllMobiles(g);
    g->notifications = createNotificationManager();
    g->timing = createTiming(g->notifications, g->player);
    g->menus = calloc(MAX_MENUS, sizeof(Menu));
    g->saveFiles = getSaveFiles();
    Fonts *fonts = createFonts(uiData);
    UISprite *uiSprite = createUISprite(findSpritesheetByName(g->sprites, uiData->sprite->name), uiData);
    MenuContext *menuContext = createMenuContext(
            NULL,
            fonts,
            uiSprite,
            g->spells->spells,
            g->saveFiles,
            NULL,
            0);
    g->ui = createUIManager(
            uiData,
            uiSprite,
            fonts,
            menuContext);
    g->fights = createFightManager(g->ui, g->spells, g->notifications);
    addDebug("done creating game object");
    addMenu(g->menus, findMenu(g->ui->menus, MAIN_MENU));
    return g;
}
