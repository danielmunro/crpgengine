typedef struct {
    const char *id;
    const char *name;
    Texture2D image;
    Attributes *attributes;
    int level;
    int hp;
    int mana;
    int gp;
    int actionGauge;
    Rectangle position;
    ActionStep step;
} Beast;

typedef struct {
    Beast *beasts[MAX_BEASTIARY_SIZE];
    int count;
} Beastiary;

Beast *createBeastFromData(BeastData *data) {
    Beast *beast = malloc(sizeof(Beast));
    beast->id = data->id;
    beast->name = data->name;
    beast->gp = data->gp;
    beast->hp = data->hp;
    beast->mana = data->mana;
    beast->level = data->level;
    beast->actionGauge = 0;
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/images/%s", runtimeArgs->indexDir, data->image);
    beast->image = LoadTextureFromImage(LoadImage(filePath));
    beast->attributes = createAttributesFromData(data->attributes);
    beast->position = (Rectangle){0, 0, 0, 0};
    beast->step = STEP_NONE;
    return beast;
}

Beast *cloneBeast(Beast *original) {
    Beast *new = malloc(sizeof(Beast));
    new->id = original->id;
    new->name = original->name;
    new->image = original->image;
    new->attributes = cloneAttributes(original->attributes);
    new->level = original->level;
    new->hp = original->hp;
    new->mana = original->mana;
    new->gp = original->gp;
    new->actionGauge = original->actionGauge;
    new->position = original->position;
    new->step = STEP_NONE;
    return new;
}

Attributes calculateBeastAttributes(Beast *beast) {
    // todo take into effect affects
    return *beast->attributes;
}

void normalizeVitalsForBeast(Beast *beast) {
    Attributes calculated = calculateBeastAttributes(beast);
    if (beast->hp > calculated.hp) {
        beast->hp = calculated.hp;
    }
    if (beast->mana > calculated.mana) {
        beast->mana = calculated.mana;
    }
}
