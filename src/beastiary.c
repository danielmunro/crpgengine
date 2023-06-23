typedef struct {
    char *id;
    char *name;
    Texture2D image;
    Attributes *attributes;
    int level;
    int hp;
    int mana;
    int gp;
} Beast;

typedef struct {
    Beast *beasts[MAX_BEASTIARY_SIZE];
    int beastCount;
} Beastiary;

Beast *createBeastFromData(const char *indexDir, BeastData *data) {
    Beast *beast = malloc(sizeof(Beast));
    beast->id = data->id;
    beast->name = data->name;
    beast->gp = data->gp;
    beast->hp = data->hp;
    beast->mana = data->mana;
    beast->level = data->level;
    char filePath[255];
    sprintf(filePath, "%s/images/%s", indexDir, data->image);
    beast->image = LoadTextureFromImage(LoadImage(filePath));
    beast->attributes = createAttributesFromData(data->attributes);
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
    return new;
}

Beastiary *createBeastiary() {
    Beastiary *beastiary = malloc(sizeof(Beastiary));
    beastiary->beastCount = 0;
    return beastiary;
}
