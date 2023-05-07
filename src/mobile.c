typedef struct Mobile {
    Animation *animations[MAX_ANIMATIONS];
    char *name;
    int direction;
    Vector2 position;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    int animName = getAnimationFromDirection(mob->direction);
    return findAnimation(mob->animations, animName);
}

Mobile *createMobile() {
    Mobile *mobile = malloc(sizeof(Mobile));
    mobile->direction = DIRECTION_DOWN;
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        mobile->animations[i] = NULL;
    }
    return mobile;
}
