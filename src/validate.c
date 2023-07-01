void validateExits(Game *g) {
    for (int i = 0; i < g->sceneCount; i++) {
        for (int j = 0; j < g->scenes[i]->exploration->exitCount; j++) {
            int found = false;
            Exit *exit = g->scenes[i]->exploration->exits[j];
            if (strcmp(exit->to, "") == 0) {
                addWarning(g->log, "exit defined without destination in '%s' scene", exit->scene);
                continue;
            }
            for (int q = 0; q < g->sceneCount; q++) {
                for (int e = 0; e < g->scenes[q]->exploration->entranceCount; e++) {
                    if (strcmp(exit->to, g->scenes[q]->exploration->entrances[e]->name) == 0) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                addError(g->log, "exit '%s' for scene '%s' does not exist",
                        exit->to,
                        g->scenes[i]->name
                );
            }
        }
    }
}

void validateGameData(Game *g) {
    validateExits(g);
}
