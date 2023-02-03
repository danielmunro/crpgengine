//
// Created by Dan Munro on 2/1/23.
//

#define MAX_DATA_SIZE 2048

char *parseSceneLayer(Tilemap *t, char *layer, int i) {
    printf("parseSceneLayer %s\n", layer);
    char *rawData = LoadFileText(layer);
    char *line = strtok(rawData, "\r\n");
    char *data[MAX_DATA_SIZE];
    int it = 0;
    while (line != NULL && it < MAX_DATA_SIZE) {
        data[it] = line;
        line = strtok(NULL, "\r\n");
        it++;
    }
    printf("data read complete\n");
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
    printf("data parse complete\n");
}

Scene *loadScene(char *sceneName) {
    printf("loading scene %s", sceneName);
    char *data = LoadFileText(sceneName);
    char *pch = NULL;
    pch = strtok(data, "\r\n");
    Scene *scene = malloc(sizeof(Scene));
    Tilemap *tilemap = malloc(sizeof(Tilemap));
    strcpy(scene->name, pch);
    pch = strtok(NULL, "\r\n");
    char *sizeStr = pch;
    pch = strtok(NULL, "\r\n");
    char *source = pch;
    pch = strtok(NULL, "\r\n");
    char *layers[MAX_LAYER_COUNT];
    int layerCount = 0;
    while (pch != NULL) {
        layers[layerCount] = pch;
        printf("adding to layers: %s\n", pch);
        pch = strtok(NULL, "\r\n");
        layerCount++;
    }
    printf("layers found: %d\n", layerCount);
    pch = strtok(sizeStr, ",");
    int height = atoi(pch);
    pch = strtok(NULL, ",");
    int width = atoi(pch);
    Vector2D size = { width, height };
    tilemap->size = size;
    printf("size: %d, %d\n", width, height);
    tilemap->source = LoadTexture(source);
    int i = 0;
    while (i < layerCount) {
        printf("parsing scene layer %s", layers[i]);
        parseSceneLayer(tilemap, layers[i], i);
        i++;
    }
    scene->layers = layerCount;
    scene->tilemap = tilemap;

    return scene;
}