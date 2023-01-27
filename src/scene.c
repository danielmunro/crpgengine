typedef struct Scene {
    struct Sprite *sprites[MAX_SPRITES];
    Texture2D background;
    Texture2D midground;
    Texture2D foreground;
} Scene;
