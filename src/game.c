#include <raylib.h>
#include <string.h>
#include "headers/util/log.h"
#include "headers/scene_manager.h"
#include "headers/map.h"
#include "headers/map_input_handler.h"
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
#include "headers/user_config.h"
#include "headers/context.h"

typedef struct {
    Context *context;
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
    SaveData *saveToLoad;
} Game;

void attemptToUseExit(Game *game, Scene *scene, const Entrance *entrance) {
    if (entrance == NULL) {
        addWarning("no entrance found for '%s' scene", scene->name);
        return;
    }
    addInfo("entrance %s found at %d, %d, %d, %d",
             entrance->name,
             entrance->area.x,
             entrance->area.y,
             entrance->area.width,
             entrance->area.height
    );
    setScene(game->scenes, scene, game->player, entrance->name);
}

void evaluateExits(Game *g) {
    const Map *m = g->scenes->current->map;
    int ex = atExit(m, g->player);
    if (ex < 0) {
        return;
    }
    const char *sceneName = m->warps->exits[ex]->scene;
    const char *entranceName = m->warps->exits[ex]->to;
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
               m->warps->exits[ex]->id, sceneName, entranceName);
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

void evaluateMoveDirection(const Game *g, Mobile *mob, Direction d) {
    mob->direction = d;
    Vector2D newPos = mob->position;
    int amountToMove = g->context->game->tileSize;
    if (d == DIRECTION_UP) {
        newPos.y -= amountToMove;
    } else if (d == DIRECTION_DOWN) {
        newPos.y += amountToMove;
    } else if (d == DIRECTION_LEFT) {
        newPos.x -= amountToMove;
    } else if (d == DIRECTION_RIGHT) {
        newPos.x += amountToMove;
    }
    if (!isCollisionDetected(g->scenes->current->map, g->player, newPos)) {
        mob->destination = newPos;
    }
}

void checkMapInput(Game *g) {
    Mobile *mob = getPartyLeader(g->player);
    if (!canPlayerMove(mob)) {
        return;
    }
    Direction d = mapCheckMoveKeys(g->player);
    if (d != -1) {
        evaluateMoveDirection(g, mob, d);
        for (int i = 0; i < g->animations->animationCount; i++) {
            if (g->animations->animations[i]->playUntil == PLAY_UNTIL_MOVE) {
                g->animations->animations[i]->isPlaying = false;
            }
        }
    }
    if (IsKeyPressed(KEY_C)) {
        mapDebugKeyPressed(mob->position, g->context->game->tileSize);
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
        const SaveFile *s = save(g->player, g->scenes->current->name, g->context->indexDir);
        addSaveFile(g->saveFiles, s);
    }
}

void initializePlayer(Game *g, const char *saveFilePath) {
    if (saveFilePath == NULL) {
        g->player = createNewPlayer(g->mobiles, g->items);
    } else {
        g->saveToLoad = loadSaveData(saveFilePath);
        g->player = mapSaveDataToPlayer(
                g->spells,
                g->animations,
                g->saveToLoad,
                g->context->game->immortal);
    }
    addMobilesToMobileManager(g->mobiles, g->player->party);
}

void checkMenuInput(Game *g) {
    Menu *menu = getCurrentMenu(g->menus);
    MenuContext *mc = g->ui->menuContext;
    mc->menuType = menu->type;
    const MenuKeyPressedType keyPressed = menu->keyPressed(mc);
    if (keyPressed == KEY_PRESSED_CLOSE_MENU) {
        removeLastMenu(g->menus, mc);
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
            const SaveFile *s = save(g->player, g->scenes->current->name, g->context->indexDir);
            addSaveFile(g->saveFiles, s);
            addMenu(g->menus, findMenu(g->ui->menus, ACKNOWLEDGE_SAVE_MENU));
        } else if (response->type == RESPONSE_TYPE_LOAD_GAME) {
            initializePlayer(g, g->saveFiles->saves[mc->cursorLine]->filename);
            removeMenu(g->menus, mc, MAIN_MENU);
        } else if (response->type == RESPONSE_TYPE_CONTINUE_GAME) {
            initializePlayer(g, g->saveFiles->saves[0]->filename);
            removeMenu(g->menus, mc, MAIN_MENU);
        } else if (response->type == RESPONSE_TYPE_NEW_GAME) {
            initializePlayer(g, NULL);
            removeMenu(g->menus, mc, MAIN_MENU);
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

bool canTriggerFight(
        const FightManager *fm,
        const Scene *s,
        const Player *p,
        bool hasRandomEncounters) {
    return isDungeon(s)
            && !isFighting(fm)
            && isMoving(getPartyLeader(p))
            && hasRandomEncounters;
}

void checkFights(Game *g, const Scene *s) {
    if (!canTriggerFight(
            g->fights,
            s,
            g->player,
            g->context->game->randomEncounters)) {
        return;
    }
    if (GetRandomValue(1, 100) == 1) {
        createFightFromEncounters(
                g->fights,
                s->encounters,
                g->player);
        Animation *animation = findAnimation(getPartyLeader(g->player)->animations, ANIMATION_LEFT);
        animation->currentFrame = animation->firstFrame;
        g->ui->menuContext->fight = g->fights->fight;
    }
}

void doExplorationLoop(Game *g) {
    Scene *s = g->scenes->current;
    checkMapInput(g);
    BeginDrawing();
    drawMapView(
            s->map,
            g->player,
            g->notifications,
            s->activeControlBlocks,
            g->ui->fonts->default_);
    processAnimations(g->animations);
    EndDrawing();
    evaluateExits(g);
    evaluateMovement(g->mobiles);
    checkControls(g->controls);
    checkFights(g, s);
}

void doFightLoop(Game *g) {
    Scene *s = g->scenes->current;
    fightUpdate(g->fights);
    if (!isFightDone(g->fights->fight)) {
        checkFightInput(g->fights);
        drawFightView(
                s->encounters,
                g->fights,
                g->context);
        checkControls(g->controls);
    }
    checkRemoveFight(g->fights);
}

void doInGameMenuLoop(Game *g) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawAllMenus(
            g->ui->menuContext,
            g->menus);
    if (g->context->game->showFPS) {
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
        int seconds = stopTiming(g->timing);
        if (seconds > 0) {
            g->player->secondsPlayed += seconds;
        }
        if (IsKeyPressed(KEY_F10)) {
            char file[MAX_FS_PATH_LENGTH];
            sprintf(file, "screenshot-%f.png", getTimeInMS());
            addInfo("saving screen shot :: %s", file);
            TakeScreenshot(file);
        }
    }
}

UIManager *initializeUIManager(Game *g) {
    UIData *uiData = loadUIData(g->context->indexDir);
    Fonts *fonts = createFonts(g->context->indexDir, uiData);
    UISprite *uiSprite = createUISprite(
            findSpritesheetByName(g->sprites, uiData->sprite->name),
            uiData);
    MenuContext *mc = createMenuContext(
            g->context,
            NULL,
            fonts,
            uiSprite,
            g->spells->spells,
            g->saveFiles,
            NULL,
            0);
    return createUIManager(
            uiData,
            uiSprite,
            fonts,
            mc);
}

void initializeGameForPlayer(Game *g) {
    g->controls = createControlManager(
            g->context,
            g->player,
            g->animations,
            g->items,
            g->notifications,
            g->mobiles,
            g->saveFiles);
    g->scenes = createSceneManager(
            g->context,
            g->controls,
            g->animations,
            g->audio);
    loadScenesFromFiles(
            g->scenes,
            g->mobiles,
            g->items,
            g->beastiary);
    setSceneBasedOnSave(g->scenes, g->player, g->saveToLoad);
    g->ui->menuContext->player = g->player;
    addDebug("game initialized for player");
}

Game *createGame(Context *c) {
    Game *g = malloc(sizeof(Game));
    g->context = c;
    g->sprites = loadSpritesheetManager(c);
    g->animations = createAnimationManager(c);
    loadAllAnimations(g->animations, g->sprites);
    g->audio = loadAudioManager(c);
    g->beastiary = loadBeastiary(c->indexDir);
    g->items = createItemManager(c);
    loadAllItems(g->items);
    g->spells = loadSpellManager(c->indexDir);
    g->mobiles = createMobileManager(g->spells, g->animations, g->context);
    loadPlayerMobiles(g->mobiles);
    g->notifications = createNotificationManager(c);
    g->menus = calloc(MAX_MENUS, sizeof(Menu));
    g->saveFiles = getSaveFiles(c->indexDir);
    g->saveToLoad = NULL;
    g->timing = createTiming(g->notifications);
    g->ui = initializeUIManager(g);
    g->fights = createFightManager(g->context, g->ui, g->spells, g->notifications);
    addMenu(g->menus, findMenu(g->ui->menus, MAIN_MENU));
    addDebug("game object created");
    return g;
}
