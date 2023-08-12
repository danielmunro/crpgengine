void dumpGame(Game *g) {
    printf("\nanimations\n==============\n");
    for (int i = 0; i < g->animationManager->libraryCount; i++) {
        printf("%s %d %d %d %d %d\n",
               g->animationManager->library[i]->name,
               g->animationManager->library[i]->type,
               g->animationManager->library[i]->isPlaying,
               g->animationManager->library[i]->currentFrame,
               g->animationManager->library[i]->frameRate,
               g->animationManager->library[i]->frameRateCount);
    }
    for (int i = 0; i < g->sceneManager->count; i++) {
        printf("\nscene %s objects\n===============\n", g->sceneManager->scenes[i]->name);
        for (int o = 0; o < g->sceneManager->scenes[i]->exploration->objectCount; o++) {
            printf("%d\n",
                   g->sceneManager->scenes[i]->exploration->objects[o]->tile
            );
        }
        printf("\nmobiles\n===============\n");
        for (int j = 0; j < g->sceneManager->scenes[i]->exploration->mobileCount; j++) {
            printf("%s\n", g->sceneManager->scenes[i]->exploration->mobiles[j]->name);
        }
        printf("\ncontrols\n==============\n");
        for (int j = 0; j < g->sceneManager->scenes[i]->controlBlockCount; j++) {
            printf("in control block %d\n", j);
            for (int w = 0; w < g->sceneManager->scenes[i]->controlBlocks[j]->whenCount; w++) {
                printf("when - %s:%d\n", g->sceneManager->scenes[i]->controlBlocks[j]->when[w]->source->name,
                       g->sceneManager->scenes[i]->controlBlocks[j]->when[w]->condition);
            }
            for (int w = 0; w < g->sceneManager->scenes[i]->controlBlocks[j]->thenCount; w++) {
                const char *target = "";
                if (g->sceneManager->scenes[i]->controlBlocks[j]->then[w]->target != NULL) {
                    target = g->sceneManager->scenes[i]->controlBlocks[j]->then[w]->target->name;
                }
                printf("then - %s:%d\n", target, g->sceneManager->scenes[i]->controlBlocks[j]->then[w]->outcome);
            }
        }
    }
}
