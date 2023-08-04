typedef struct {
    Rectangle area;
    char *to;
    char *scene;
} Exit;

typedef struct {
    char *name;
    Rectangle area;
    AnimationType direction;
} Entrance;