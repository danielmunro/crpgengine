typedef struct Scene {
    Sprite *sprites[MAX_SPRITES];
    Tilemap *tilemap;
} Scene;

Scene *createTestScene() {
    Scene *scene = malloc(sizeof(Scene));
    scene->tilemap = createTestTilemap();
    return scene;
}
