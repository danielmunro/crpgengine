void dumpGame(Game *g) {
    printf("animations\n==============\n");
    for (int i = 0; i < g->animIndex; i++) {
        printf("%d %d %d %d %d\n",
               g->animations[i]->type,
               g->animations[i]->isPlaying,
               g->animations[i]->currentFrame,
               g->animations[i]->frameRate,
               g->animations[i]->frameRateCount);
    }
    for (int i = 0; i < MAX_SCENES; i++) {
        if (g->scenes[i] == NULL) {
            break;
        }
        printf("scene %s objects\n===============\n", g->scenes[i]->name);
        for (int o = 0; o < MAX_OBJECTS; o++) {
            if (g->scenes[i]->objects[o] == NULL) {
                break;
            }
            printf("%d %d\n",
                   g->scenes[i]->objects[o]->id,
                   g->scenes[i]->objects[o]->tile
            );
        }
        printf("mobiles\n===============\n");
        for (int j = 0; j < MAX_CONTROLS; j++) {
            if (g->scenes[i]->controlBlocksInt[j] == NULL) {
                break;
            }
            for (int w = 0; w < g->scenes[i]->controlBlocksInt[j]->whenCount; w++) {
                printf("when - %s:%s\n", g->scenes[i]->controlBlocksInt[j]->when[w][0], g->scenes[i]->controlBlocksInt[j]->when[w][1]);
            }
            for (int w = 0; w < g->scenes[i]->controlBlocksInt[j]->thenCount; w++) {
                printf("then - %s:%s\n", g->scenes[i]->controlBlocksInt[j]->then[w][0], g->scenes[i]->controlBlocksInt[j]->then[w][1]);
            }
        }
    }
}
