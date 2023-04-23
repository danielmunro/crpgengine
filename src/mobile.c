typedef struct Mobile {
    Animation *animations[MAX_ANIMATIONS];
    int direction;
    int isMoving;
    Vector2 position;
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    int animName = getAnimationFromDirection(mob->direction);
    return findAnimation(mob->animations, animName);
}
