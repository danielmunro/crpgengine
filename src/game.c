typedef struct Game {
    struct Player *player;
    struct Sprite *sp;
    struct Scene *scenes[MAX_SCENES];
} Game;
