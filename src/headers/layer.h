#ifndef CJRPGENGINE_LAYER_H
#define CJRPGENGINE_LAYER_H

#define MAX_LAYERS 1024
#define MAX_LAYER_SIZE 1024
#define LAYER_COUNT sizeof(LAYERS) / sizeof(LAYERS[0])

#include <string.h>
#include "src/headers/util/log.h"
#include "src/headers/errors.h"

typedef enum {
    BACKGROUND = 0,
    MIDGROUND,
    FOREGROUND,
} LayerType;

LayerType LAYERS[] = {
        BACKGROUND,
        MIDGROUND,
        FOREGROUND,
};

const char *LayerTypes[] = {
        "background",
        "midground",
        "foreground",
};

LayerType getLayerTypeFromString(const char *layerType) {
    for (int i = 0; i < LAYER_COUNT; i++) {
        if (strcmp(LayerTypes[i], layerType) == 0) {
            return i;
        }
    }
    addError("layer type not found :: %s", layerType);
    exit(ConfigurationErrorLayerTypeNotFound);
}

#endif //CJRPGENGINE_LAYER_H
