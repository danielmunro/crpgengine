//
// Created by Dan Munro on 2/1/23.
//

#define MAX_DATA_SIZE 2048

void parseSceneLayer(Tilemap *t, char *layer, int i) {
    char *rawData = LoadFileText(layer);
    char *line = strtok(rawData, "\r\n");
    char *data[MAX_DATA_SIZE];
    int it = 0;
    while (line != NULL && it < MAX_DATA_SIZE) {
        data[it] = line;
        line = strtok(NULL, "\r\n");
        it++;
    }
    int y = 0, x = 0;
    while (y < it) {
        char *val = strtok(data[y], ",");
        x = 0;
        while (val != NULL) {
            t->layers[i][y][x] = atoi(val);
            val = strtok(NULL, ",");
            x++;
        }
        y++;
    }
}

void parseTilemapLayers(Tilemap *t, char *layers[MAX_LAYER_COUNT], int layerCount) {
    int i = 0;
    while (i < layerCount) {
        printf("parsing scene layer %s\n", layers[i]);
        parseSceneLayer(t, layers[i], i);
        i++;
    }
}

void assignSceneType(Scene *s, char *sceneType) {
    int i = 0, count = sizeof(sceneTypes) / sizeof(SceneType);
    while (i <= count) {
        if (strcmp(sceneTypes[i].scene, sceneType) == 0) {
            s->type = sceneTypes[i].code;
            break;
        }
        i++;
    }
}

Scene *loadScene(char *sceneName) {
    printf("loading scene %s", sceneName);
    char *data = LoadFileText(sceneName);
    Scene *scene = malloc(sizeof(Scene));
    strcpy(scene->name, strtok(data, "\r\n"));
    char *sizeStr = strtok(NULL, "\r\n");
    char *source = strtok(NULL, "\r\n");
    char *sceneType = strtok(NULL, "\r\n");
    char *layers[MAX_LAYER_COUNT];
    int layerCount = 0;
    char *pch = strtok(NULL, "\r\n");
    while (pch != NULL) {
        layers[layerCount] = pch;
        printf("adding to layers: %s\n", pch);
        pch = strtok(NULL, "\r\n");
        layerCount++;
    }
    printf("layers found: %d\n", layerCount);

    // size
    int height = atoi(strtok(sizeStr, ","));
    int width = atoi(strtok(NULL, ","));
    Vector2D size = { width, height };

    // create tilemap
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    tilemap->size = size;
    tilemap->source = LoadTexture(source);
    parseTilemapLayers(tilemap, layers, layerCount);

    // assign scene properties
    assignSceneType(scene, sceneType);
    scene->layers = layerCount;
    scene->tilemap = tilemap;
    printf("done parsing scene %s\n", sceneName);

    return scene;
}