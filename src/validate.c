void validateExits(Game *g) {
    for (int i = 0; i < g->sceneManager->count; i++) {
        addDebug(g->log, "scene '%s' exit count: %d",
                 g->sceneManager->scenes[i]->name,
                 g->sceneManager->scenes[i]->exploration->exitCount);
        for (int j = 0; j < g->sceneManager->scenes[i]->exploration->exitCount; j++) {
            int found = false;
            Exit *exit = g->sceneManager->scenes[i]->exploration->exits[j];
            if (strcmp(exit->to, "") == 0) {
                addWarning(g->log, "exit defined without destination in '%s' scene", g->sceneManager->scenes[i]->name);
                continue;
            }
            for (int q = 0; q < g->sceneManager->count; q++) {
                for (int e = 0; e < g->sceneManager->scenes[q]->exploration->entranceCount; e++) {
                    if (strcmp(exit->to, g->sceneManager->scenes[q]->exploration->entrances[e]->name) == 0) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                addError(g->log, "exit '%s' for scene '%s' does not exist",
                        exit->to,
                        g->sceneManager->scenes[i]->name
                );
            }
        }
    }
}

void validateGameData(Game *g) {
    addDebug(g->log, "validating game data");
    validateExits(g);
}
