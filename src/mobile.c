typedef struct {
    Animation *animations[MAX_ANIMATIONS];
    const char *id;
    const char *name;
    int direction;
    Vector2 position;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    return findAnimation(mob->animations, mob->direction);
}

Mobile *createMobile(const char *id, const char *name, Vector2 position, AnimationDirection direction, Animation *animations[MAX_ANIMATIONS]) {
    Mobile *mobile = malloc(sizeof(Mobile));
    mobile->id = &id[0];
    mobile->name = &name[0];
    mobile->direction = direction;
    mobile->position = position;
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        mobile->animations[i] = animations[i];
    }
    return mobile;
}
