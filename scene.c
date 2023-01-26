#define MAX_SPRITES 128

struct Scene {
    Sprite *sprites[MAX_SPRITES];
    Texture2D background;
    Texture2D midground;
    Texture2D foreground;
};
