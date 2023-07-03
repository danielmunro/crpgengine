typedef struct {
    Animation *animations[MAX_ANIMATIONS];
    const char *id;
    const char *name;
    AnimationDirection direction;
    Vector2 position;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    return findAnimation(mob->animations, mob->direction);
}

Mobile *createMobile(const char *id, const char *name, Vector2 position, AnimationDirection direction,
                     Animation *animations[MAX_ANIMATIONS]) {
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

Mobile *createMobileFromData(MobileData *data, Animation *animations[MAX_ANIMATIONS]) {
    Mobile *mob = createMobile(
            data->id,
            data->name,
            (Vector2) {(float) data->position[0], (float) data->position[1]},
            getDirectionFromString(data->direction),
            animations);
    return mob;
}

Rectangle getMobRectangle(Mobile *mob) {
    return (Rectangle) {
            mob->position.x,
            mob->position.y + MOB_COLLISION_HEIGHT,
            MOB_COLLISION_WIDTH,
            MOB_COLLISION_HEIGHT,
    };
}
