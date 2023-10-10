#include <mm_malloc.h>
#include <raylib.h>
#include <string.h>
#include "src/headers/log.h"
#include "src/headers/collision.h"

typedef struct {
    int sceneIndex;
    bool showObjectCollisions;
    bool showPlayerCollision;
    bool showWarpCollisions;
    bool dump;
    bool exit;
    bool forceNewGame;
    bool logMemory;
    bool purgeSaves;
    bool showFPS;
    LogLevel logLevel;
    const char *indexDir;
    const char *saveFile;
} RuntimeArgs;

RuntimeArgs *runtimeArgs;

RuntimeArgs *createRuntimeArgs(int argc, char *argv[]) {
    runtimeArgs = malloc(sizeof(RuntimeArgs));
    runtimeArgs->sceneIndex = -1;
    runtimeArgs->showObjectCollisions = false;
    runtimeArgs->showPlayerCollision = false;
    runtimeArgs->showWarpCollisions = false;
    runtimeArgs->dump = false;
    runtimeArgs->exit = false;
    runtimeArgs->logLevel = INFO;
    runtimeArgs->indexDir = NULL;
    runtimeArgs->saveFile = NULL;
    runtimeArgs->forceNewGame = false;
    runtimeArgs->logMemory = false;
    runtimeArgs->purgeSaves = false;
    runtimeArgs->showFPS = false;
    for (int i = 1; i < argc; i++) {
        if (argv[i][0] == '-') {
            if (argv[i][1] == 'c') {
                char *collider = strtok(argv[i + 1], ",");
                while (collider != NULL) {
                    CollisionType collisionType = getCollisionTypeFromString(collider);
                    if (collisionType == COLLISION_TYPE_OBJECTS) {
                        runtimeArgs->showObjectCollisions = true;
                    } else if (collisionType == COLLISION_TYPE_PLAYER) {
                        runtimeArgs->showPlayerCollision = true;
                    } else if (collisionType == COLLISION_TYPE_WARPS) {
                        runtimeArgs->showWarpCollisions = true;
                    }
                    collider = strtok(NULL, ",");
                }
                free(collider);
            } else if (argv[i][1] == 's') {
                runtimeArgs->sceneIndex = TextToInteger(argv[i + 1]);
            } else if (argv[i][1] == 'd') {
                runtimeArgs->dump = true;
            } else if (argv[i][1] == 'e') {
                runtimeArgs->exit = true;
            } else if (argv[i][1] == 'i') {
                runtimeArgs->indexDir = &argv[i + 1][0];
            } else if (argv[i][1] == 'l') {
                runtimeArgs->logLevel = getLogLevelFromString(&argv[i + 1][0]);
                if (runtimeArgs->logLevel == -1) {
                    fprintf(stderr, "log level must be one of [debug, info, warn, error], setting to info\n");
                    runtimeArgs->logLevel = INFO;
                }
            } else if (argv[i][1] == 'v') {
                runtimeArgs->saveFile = &argv[i + 1][0];
            } else if (argv[i][1] == 'n') {
                runtimeArgs->forceNewGame = true;
            } else if (argv[i][1] == 'm') {
                runtimeArgs->logMemory = true;
            } else if (strcmp(&argv[i][1], "ps") == 0) {
                runtimeArgs->purgeSaves = true;
            } else if (argv[i][1] == 'f') {
                runtimeArgs->showFPS = true;
            }
        }
    }
    if (runtimeArgs->indexDir == NULL) {
        addError("indexDir (-i) argument is required");
        exit(ArgumentErrorIndexDirectoryRequired);
    }
    if (runtimeArgs->forceNewGame && runtimeArgs->saveFile != NULL) {
        addError("cannot specify -n and -v together");
        exit(ArgumentErrorForceNewAndLoadSave);
    }
    return runtimeArgs;
}
