void dumpGame(Game *g) {
    printf("\nanimations\n==============\n");
    for (int i = 0; i < g->animIndex; i++) {
        printf("%d %d %d %d %d\n",
               g->animations[i]->type,
               g->animations[i]->isPlaying,
               g->animations[i]->currentFrame,
               g->animations[i]->frameRate,
               g->animations[i]->frameRateCount);
    }
    for (int i = 0; i < g->sceneCount; i++) {
        printf("\nscene %s objects\n===============\n", g->scenes[i]->name);
        for (int o = 0; o < MAX_OBJECTS; o++) {
            if (g->scenes[i]->exploration->objects[o] == NULL) {
                break;
            }
            printf("%d %d\n",
                   g->scenes[i]->exploration->objects[o]->id,
                   g->scenes[i]->exploration->objects[o]->tile
            );
        }
        printf("\nmobiles\n===============\n");
        for (int j = 0; j < MAX_MOBILES; j++) {
            if (g->scenes[i]->exploration->mobiles[j] == NULL) {
                break;
            }
            printf("%s\n", g->scenes[i]->exploration->mobiles[j]->name);
        }
        printf("\ncontrols\n==============\n");
        for (int j = 0; j < MAX_CONTROLS; j++) {
            if (g->scenes[i]->controlBlocks[j] == NULL) {
                break;
            }
            printf("in control block %d\n", j);
            for (int w = 0; w < g->scenes[i]->controlBlocks[j]->whenCount; w++) {
                printf("when - %s:%d\n", g->scenes[i]->controlBlocks[j]->when[w]->source->name, g->scenes[i]->controlBlocks[j]->when[w]->condition);
            }
            for (int w = 0; w < g->scenes[i]->controlBlocks[j]->thenCount; w++) {
                char *target = "";
                if (g->scenes[i]->controlBlocks[j]->then[w]->target != NULL) {
                    target = g->scenes[i]->controlBlocks[j]->then[w]->target->name;
                }
                printf("then - %s:%d\n", target, g->scenes[i]->controlBlocks[j]->then[w]->outcome);
            }
        }
    }
}
