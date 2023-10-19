void dumpGame(Game *g) {
    printf("\nanimations\n==============\n");
    for (int i = 0; i < g->animations->libraryCount; i++) {
        printf("%s %d %d %d %d %d\n",
               g->animations->library[i]->name,
               g->animations->library[i]->type,
               g->animations->library[i]->isPlaying,
               g->animations->library[i]->currentFrame,
               g->animations->library[i]->frameRate,
               g->animations->library[i]->frameRateCount);
    }
    for (int i = 0; i < g->scenes->count; i++) {
        printf("\nscene %s tiles\n===============\n", g->scenes->scenes[i]->name);
        for (int t = 0; t < g->scenes->scenes[i]->exploration->tilesCount; t++) {
            printf("%d\n",
                   g->scenes->scenes[i]->exploration->tiles[t]->id
            );
        }
        printf("\nmobiles\n===============\n");
        for (int j = 0; j < g->scenes->scenes[i]->exploration->mobileCount; j++) {
            printf("%s\n", g->scenes->scenes[i]->exploration->mobiles[j]->name);
        }
        printf("\ncontrols\n==============\n");
        for (int j = 0; j < g->scenes->scenes[i]->controlBlockCount; j++) {
            printf("in control block %d\n", j);
            for (int w = 0; w < g->scenes->scenes[i]->controlBlocks[j]->whenCount; w++) {
                printf("when - %s:%d\n", g->scenes->scenes[i]->controlBlocks[j]->when[w]->source->name,
                       g->scenes->scenes[i]->controlBlocks[j]->when[w]->condition);
            }
            for (int w = 0; w < g->scenes->scenes[i]->controlBlocks[j]->thenCount; w++) {
                const char *target = "";
                if (g->scenes->scenes[i]->controlBlocks[j]->then[w]->target != NULL) {
                    target = g->scenes->scenes[i]->controlBlocks[j]->then[w]->target->name;
                }
                printf("then - %s:%d\n", target, g->scenes->scenes[i]->controlBlocks[j]->then[w]->outcome);
            }
        }
    }
}
