#ifndef CJRPGENGINE_LAYER_H
#define CJRPGENGINE_LAYER_H

#define MAX_LAYERS 1024
#define MAX_LAYER_SIZE 1024

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

#define LAYER_COUNT sizeof(LAYERS) / sizeof(LAYERS[0])

#endif //CJRPGENGINE_LAYER_H
