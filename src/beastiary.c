typedef struct Beast {
    char *id;
    char *name;
    Texture2D image;
    Attributes *attributes;
    int level;
    int hp;
    int mana;
    int gp;
} Beast;

typedef struct Beastiary {
    Beast *beasts[MAX_BEASTIARY_SIZE];
    int beastCount;
} Beastiary;

Beast *createBeast() {
    Beast *b = malloc(sizeof(Beast));
    b->id = "";
    b->name = "";
    b->level = 0;
    b->hp = 0;
    b->mana = 0;
    b->gp = 0;
    return b;
}

Beast *createBeastFromData(const char *indexDir, BeastData *data) {
    Beast *beast = createBeast();
    beast->id = data->id;
    beast->name = data->name;
    beast->gp = data->gp;
    beast->hp = data->hp;
    beast->mana = data->mana;
    beast->level = data->level;
    char *filepath = pathCat(indexDir, pathCat("/images/", data->image));
    beast->image = LoadTextureFromImage(LoadImage(filepath));
    beast->attributes = createAttributesFromData(data->attributes);
    return beast;
}

Beast *cloneBeast(Beast *original) {
    Beast *new = createBeast();
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
