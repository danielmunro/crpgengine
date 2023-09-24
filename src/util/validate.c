void validateExits(Game *g) {
    for (int i = 0; i < g->scenes->count; i++) {
        addDebug("scene '%s' exit count: %d",
                 g->scenes->scenes[i]->name,
                 g->scenes->scenes[i]->exploration->exitCount);
        for (int j = 0; j < g->scenes->scenes[i]->exploration->exitCount; j++) {
            int found = false;
            Exit *exit = g->scenes->scenes[i]->exploration->exits[j];
            if (strcmp(exit->to, "") == 0) {
                addWarning("exit defined without destination in '%s' scene", g->scenes->scenes[i]->name);
                continue;
            }
            for (int q = 0; q < g->scenes->count; q++) {
                for (int e = 0; e < g->scenes->scenes[q]->exploration->entranceCount; e++) {
                    if (strcmp(exit->to, g->scenes->scenes[q]->exploration->entrances[e]->name) == 0) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                addError("exit '%s' for scene '%s' does not exist",
                         exit->to,
                         g->scenes->scenes[i]->name
                );
            }
        }
    }
}

void validateGameData(Game *g) {
    addDebug("validating game data");
    validateExits(g);
}
