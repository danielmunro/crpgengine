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
    }
}
