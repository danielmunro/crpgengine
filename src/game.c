typedef struct Game {
    Scene *scenes[MAX_SCENES];
    int sceneCount;
    Scene *currentScene;
    Player *player;
    Animation *animations[MAX_ANIMATIONS_IN_GAME];
    AudioManager *audioManager;
    int animIndex;
} Game;

void addAnimation(Game *g, Animation *a) {
    g->animations[g->animIndex] = a;
    g->animIndex++;
}

void addAllAnimations(Game *g, Animation *animations[MAX_ANIMATIONS]) {
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        if (animations[i] == NULL) {
            break;
        }
        addAnimation(g, animations[i]);
    }
}

void setScene(Game *g, Scene *scene) {
    printf("request scene set to %s\n", scene->name);
    if (g->currentScene != NULL) {
        free(g->currentScene);
    }
//    g->currentScene = createScene();
//    strcpy(g->currentScene->name, scene->name);
//    g->currentScene->tilemap = scene->tilemap;
//    g->currentScene->entrance = scene->entrance;
//    g->currentScene->showCollisions = scene->showCollisions;
//    g->currentScene->layerCount = scene->layerCount;
//    g->currentScene->music = scene->music;
//    for (int i = 0; i < MAX_EXITS; i++) {
//        g->currentScene->exits[i] = scene->exits[i];
//    }
//    for (int i = 0; i < MAX_OBJECTS; i++) {
//        g->currentScene->objects[i] = scene->objects[i];
//    }
//    for (int i = 0; i < LAYER_COUNT; i++) {
//        g->currentScene->layers[i] = scene->layers[i];
//    }
//    for (int i = 0; i < MAX_MOBILES; i++) {
//        g->currentScene->mobiles[i] = scene->mobiles[i];
//    }
    g->currentScene = scene;

    memset(g->animations, 0, sizeof(g->animations));
    g->animIndex = 0;
    addAllAnimations(g, g->player->mob->animations);
    Rectangle r = g->currentScene->entrance;
    g->player->pos.x = r.x + (r.width / 2);
    g->player->pos.y = r.y + (r.height / 2);
    drawScene(g->currentScene);
    printf("current scene: %s\n", g->currentScene->name);
    printf("play music %s\n", g->currentScene->music);
    playMusic(g->audioManager, g->currentScene->music);
    printf("scene set to %s\n", g->currentScene->name);
}

int mapCondition(char *when) {
    if (strcmp(when, WHEN_HAS) == 0) {
        return CONDITION_HAS;
    } else if (strcmp(when, WHEN_NOT_HAS) == 0) {
        return CONDITION_NOT_HAS;
    } else if (strcmp(when, WHEN_ENGAGED) == 0) {
        return CONDITION_ENGAGED;
    } else if (strcmp(when, WHEN_AT) == 0) {
        return CONDITION_AT;
    } else if (strcmp(when, WHEN_HAS_STORY) == 0) {
        return CONDITION_HAS_STORY;
    } else if (strcmp(when, WHEN_NOT_HAS_STORY) == 0) {
        return CONDITION_NOT_HAS_STORY;
    }
    return -1;
}

int mapOutcome(char *then) {
    if (strcmp(then, THEN_SPEAK) == 0) {
        return OUTCOME_SPEAK;
    } else if (strcmp(then, THEN_MOVE_TO) == 0) {
        return OUTCOME_MOVE_TO;
    } else if (strcmp(then, THEN_DIRECTION) == 0) {
        return OUTCOME_DIRECTION;
    } else if (strcmp(then, THEN_SPRITE) == 0) {
        return OUTCOME_SPRITE;
    } else if (strcmp(then, THEN_WAIT) == 0) {
        return OUTCOME_WAIT;
    } else if (strcmp(then, THEN_GIVE_ITEM) == 0) {
        return OUTCOME_GIVE_ITEM;
    } else if (strcmp(then, THEN_TAKE) == 0) {
        return OUTCOME_TAKE;
    } else if (strcmp(then, THEN_ADD_STORY) == 0) {
        return OUTCOME_ADD_STORY;
    }
    return -1;
}

Mobile *findMobById(Game *g, char *id) {
    for (int s = 0; s < g->sceneCount; s++) {
        for (int m = 0; m < g->scenes[s]->mobileCount; m++) {
            if (strcmp(g->scenes[s]->mobiles[m]->id, id) == 0) {
                return g->scenes[s]->mobiles[m];
            }
        }
    }
    fprintf(stderr, "mob not found: %s\n", id);
    return NULL;
}

ControlBlock *mapStorylineToControlBlock(Game *g, StorylineData *storyline) {
    ControlBlock *c = createControlBlock(CONTROL_TYPE_WHEN);
    c->whenCount = storyline->when_count;
    c->thenCount = storyline->then_count;
    printf("when count: %d\n", storyline->when_count);
    for (int i = 0; i < storyline->when_count; i++) {
        c->when[i] = createWhen();
        printf("condition: %s\n", storyline->when[i].condition);
        c->when[i]->condition = mapCondition(storyline->when[i].condition);
        printf("mapped condition: %d\n", c->when[i]->condition);
        printf("story: %s\n", storyline->when[i].story);
        if (storyline->when[i].story != NULL) {
            c->when[i]->story = storyline->when[i].story;
        }
        c->when[i]->source = g->player->mob;
        if (storyline->when[i].mob != NULL) {
            printf("mobileTrigger: %s\n", storyline->when[i].mob);
            if (strcmp(storyline->when[i].mob, "true") == 0) {
                c->when[i]->mobileTrigger = storyline->mob;
            } else {
                c->when[i]->mobileTrigger = findMobById(g, storyline->when[i].mob);
            }
            printf("mob: %s\n", c->when[i]->mobileTrigger->name);
        }
    }
    printf("then count: %d\n", storyline->then_count);
    for (int i = 0; i < storyline->then_count; i++) {
        c->then[i] = createThen();
        if (storyline->then[i].player) {
            printf("player is target\n");
            c->then[i]->target = g->player->mob;
        } else {
            printf("mob %s is target\n", storyline->then[i].mob);
            if (strcmp(storyline->then[i].mob, "true") == 0) {
                c->then[i]->target = storyline->mob;
            } else {
                c->then[i]->target = findMobById(g, storyline->then[i].mob);
            }
        }
        c->then[i]->story = &storyline->then[i].story[0];
        c->then[i]->message = &storyline->then[i].message[0];
        c->then[i]->outcome = mapOutcome(storyline->then[i].action);
        printf("story: %s\n", c->then[i]->story);
        printf("message: %s\n", c->then[i]->message);
        printf("outcome: %d\n", c->then[i]->outcome);
    }
    return c;
}

void loadScenes(Game *g, int showCollisions, char *indexDir, char *scenes[MAX_SCENES]) {
    for (int i = 0; i < g->sceneCount; i++) {
        g->scenes[i] = loadScene(indexDir, scenes[i], showCollisions);
    }
    for (int i = 0; i < g->sceneCount; i++) {
        for (int c = 0; c < g->scenes[i]->storylineCount; c++) {
            g->scenes[i]->controlBlocks[c] = mapStorylineToControlBlock(g, g->scenes[i]->storylines[c]);
        }
    }
}

void processAnimations(Game *g) {
    for (int i = 0; i < g->animIndex; i++) {
        if (g->animations[i] != NULL && g->animations[i]->isPlaying) {
            incrementAnimFrame(g->animations[i]);
        }
    }
}

void evaluateExits(Game *g) {
    Scene *s = g->currentScene;
    int exit = atExit(s, g->player);
    if (exit > -1) {
        char *to = s->exits[exit]->to;
        for (int i = 0; i < g->sceneCount; i++) {
            if (strcmp(to, g->scenes[i]->name) == 0) {
                g->player->mob->position = (Vector2) {
                    (float) s->exits[exit]->x,
                    (float) s->exits[exit]->y
                };
                setScene(g, g->scenes[i]);
                break;
            }
        }
        fprintf(stderr, "warp to %s not found\n", to);
    }
}

void run(Game *g) {
    while (!WindowShouldClose()) {
        checkInput(g->currentScene, g->player);
        BeginDrawing();
        renderScene(g->currentScene, g->player);
        drawControl(g->player, g->currentScene->activeControlBlock);
        EndDrawing();
        processAnimations(g);
        evaluateMovement(g->currentScene, g->player);
        evaluateExits(g);
        controlWhenCheck(g->currentScene, g->player);
        controlThen(g->currentScene, g->player);
        activeControlRemoveCheck(g->currentScene);
        UpdateMusicStream(g->audioManager->music[g->audioManager->musicIndex]->music);
    }
}

Game *createGame(RuntimeArgs *r) {
    Game *g = malloc(sizeof(Game));
    g->animIndex = 0;
    g->currentScene = NULL;
    g->audioManager = loadAudioManager(r->indexDir);
    g->player = loadPlayer(r->indexDir);
    char *scenes[MAX_SCENES];
    char *sceneDir = pathCat(r->indexDir, "/scenes");
    g->sceneCount = getFilesInDirectory(sceneDir, scenes);
    loadScenes(g, r->showCollisions, r->indexDir, scenes);
    setScene(g, g->scenes[r->sceneIndex]);
    return g;
}
