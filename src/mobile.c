typedef struct Mobile {
    Animation *animations[MAX_ANIMATIONS];
    int direction;
    Vector2 position;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    int animName = getAnimationFromDirection(mob->direction);
    return findAnimation(mob->animations, animName);
}

Mobile *createMobile() {
    Mobile *mobile = malloc(sizeof(Mobile));
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        mobile->animations[i] = NULL;
    }
    return mobile;
}

Mobile *createTestHumanoid() {
    Mobile *mob = createMobile();
    mob->direction = DIRECTION_DOWN;
    mob->position = (Vector2){ ((float)SCREEN_WIDTH / 2) - (HUMANOID_WIDTH / 2), ((float)SCREEN_HEIGHT / 2) - (HUMANOID_HEIGHT / 2) };
    return mob;
}
