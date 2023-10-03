void validateExits(Game *g) {
    for (int i = 0; i < g->scenes->count; i++) {
        addDebug("scene exit count :: %s - %d",
                 g->scenes->scenes[i]->name,
                 g->scenes->scenes[i]->exploration->exitCount);
        for (int j = 0; j < g->scenes->scenes[i]->exploration->exitCount; j++) {
            int found = false;
            Exit *ex = g->scenes->scenes[i]->exploration->exits[j];
            if (strcmp(ex->to, "") == 0) {
                addWarning(
                        "exit defined without destination in scene :: %s",
                        g->scenes->scenes[i]->name);
                continue;
            }
            for (int q = 0; q < g->scenes->count; q++) {
                for (int e = 0; e < g->scenes->scenes[q]->exploration->entranceCount; e++) {
                    if (strcmp(ex->to, g->scenes->scenes[q]->exploration->entrances[e]->name) == 0) {
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                addError("exit for scene does not exist :: %s, %s",
                         ex->to,
                         g->scenes->scenes[i]->name);
                exit(RuntimeErrorUnknownWarp);
            }
        }
    }
}

void validateGameData(Game *g) {
    addDebug("validating game data");
    validateExits(g);
}
