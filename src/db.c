//
// Created by Dan Munro on 2/1/23.
//

Scene *loadScene() {
    char *data = LoadFileText("./resources/firsttown.scene");
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
    char *layers[24];
    int layer = 0;
    while (pch != NULL) {
        layers[layer] = pch;
        pch = strtok(NULL, "\r\n");
        layer++;
    }
    pch = strtok(sizeStr, ",");
    int height = atoi(pch);
    pch = strtok(NULL, ",");
    int width = atoi(pch);
    Vector2D size = { height, width };
    tilemap->size = size;
    tilemap->source = LoadTexture(source);

    return scene;
}