typedef struct {
    Animation *animations[MAX_ANIMATIONS];
    char *id;
    char *name;
    int direction;
    Vector2 position;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    return findAnimation(mob->animations, mob->direction);
}

Mobile *createMobile() {
    Mobile *mobile = malloc(sizeof(Mobile));
    mobile->direction = DOWN;
    mobile->position = (Vector2) {0, 0 };
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        mobile->animations[i] = NULL;
    }
    return mobile;
}
