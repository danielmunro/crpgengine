#include "headers/util/log.h"

bool doesExitHaveEntrance(const Game *g, const Exit *e) {
    for (int i = 0; i < g->scenes->count; i++) {
        for (int j = 0; j < g->scenes->scenes[i]->map->warps->entranceCount; j++) {
            if (strcmp(e->to, g->scenes->scenes[i]->map->warps->entrances[j]->name) == 0) {
                return true;
            }
        }
    }
    return false;
}

void validateExits(Game *g) {
    for (int i = 0; i < g->scenes->count; i++) {
        addDebug("scene exit count :: %s - %d",
                 g->scenes->scenes[i]->name,
                 g->scenes->scenes[i]->map->warps->exitCount);
        for (int j = 0; j < g->scenes->scenes[i]->map->warps->exitCount; j++) {
            Exit *ex = g->scenes->scenes[i]->map->warps->exits[j];
            if (strcmp(ex->to, "") == 0) {
                addWarning(
                        "exit defined without destination in scene :: %s",
                        g->scenes->scenes[i]->name);
                continue;
            }
            if (!doesExitHaveEntrance(g, ex)) {
                addError("exit for scene does not exist :: %s, %s",
                         ex->to,
                         g->scenes->scenes[i]->name);
                exit(ConfigurationErrorUnknownWarp);
            }
        }
    }
}

void validateMobileStorylineReferences(Game *g) {
    int mobsFound = 0;
    for (int i = 0; i < g->scenes->count; i++) {
        Scene *s = g->scenes->scenes[i];
        addDebug("validate scene mobiles :: %s", s->name);
        for (int j = 0; j < s->storylineCount; j++) {
            for (int q = 0; q < s->storylines[j]->when_count; q++) {
                if (s->storylines[j]->when[q].mob != NULL) {
                    const char *id = s->storylines[j]->when[q].mob;
                    findMobById(g->mobiles, id);
                    mobsFound++;
                }
            }
            for (int q = 0; q < s->storylines[j]->then_count; q++) {
                if (s->storylines[j]->then[q].mob != NULL) {
                    const char *id = s->storylines[j]->then[q].mob;
                    findMobById(g->mobiles, id);
                    mobsFound++;
                }
            }
        }
    }
    addDebug("all mobs validated :: %d total mob references", mobsFound);
}

void validateGameData(Game *g) {
    addDebug("validate game data");
    validateExits(g);
    validateMobileStorylineReferences(g);
}
