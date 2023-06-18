void validateExits(Game *g) {
    for (int i = 0; i < g->sceneCount; i++) {
        for (int j = 0; j < MAX_EXITS; j++) {
            if (g->scenes[i]->exploration->exits[j] == NULL) {
                break;
            }
            int found = false;
            for (int q = 0; q < g->sceneCount; q++) {
                if (g->scenes[q] == NULL) {
                    break;
                }
                if (strcmp(g->scenes[i]->exploration->exits[j]->to, g->scenes[q]->name) == 0) {
                    found = true;
                }
            }
            if (!found) {
                fprintf(stderr, "exit '%s' for scene '%s' does not exist\n",
                        g->scenes[i]->exploration->exits[j]->to,
                        g->scenes[i]->name
                );
            }
        }
    }
}

void validateGameData(Game *g) {
    validateExits(g);
}
