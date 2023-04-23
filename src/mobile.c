typedef struct Mobile {
    Animation *animations[MAX_ANIMATIONS];
    int direction;
    int isMoving;
    Vector2 position;
} Mobile;
