typedef enum {
    BACKGROUND = 0,
    MIDGROUND,
    FOREGROUND,
} LayerType;

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
