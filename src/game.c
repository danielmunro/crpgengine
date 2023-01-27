typedef struct Game {
    struct Player *player;
    struct Scene *scenes[MAX_SCENES];
    int scene;
} Game;
