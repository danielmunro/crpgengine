typedef struct Mobile {
    Animation *animations[MAX_ANIMATIONS];
    char *id;
    char *name;
    int direction;
    int gp;
    int level;
    int experienceToLevel;
    int hp;
    int mana;
    Vector2 position;
    Attributes *attributes;
    Item *items[MAX_ITEMS];
} Mobile;

Animation *getMobAnimation(Mobile *mob) {
    int animName = getAnimationFromDirection(mob->direction);
    return findAnimation(mob->animations, animName);
}

Mobile *createMobile() {
    Mobile *mobile = malloc(sizeof(Mobile));
    mobile->direction = DIRECTION_DOWN;
    mobile->position = (Vector2) {0, 0 };
    for (int i = 0; i < MAX_ANIMATIONS; i++) {
        mobile->animations[i] = NULL;
    }
    mobile->attributes = createAttributes();
    mobile->gp = 0;
    mobile->level = 1;
    mobile->experienceToLevel = 0;
    mobile->hp = 1;
    mobile->mana = 0;
    return mobile;
}
