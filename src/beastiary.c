typedef struct Beast {
    char *id;
    char *name;
    Image image;
    Attributes *attributes;
    int level;
    int hp;
    int mana;
    int gp;
} Beast;

typedef struct Beastiary {
    Beast *beasts[MAX_BEASTIARY_SIZE];
} Beastiary;

Beast *createBeastFromData(const char *indexDir, BeastData *data) {
    Beast *beast = malloc(sizeof(Beast));
    beast->id = data->id;
    beast->name = data->name;
    beast->gp = data->gp;
    beast->hp = data->hp;
    beast->mana = data->mana;
    beast->level = data->level;
    char *filepath = pathCat(indexDir, pathCat("/images/", data->image));
    beast->image = LoadImage(filepath);
    beast->attributes = createAttributesFromData(data->attributes);
    return beast;
}

Beastiary *createBeastiary() {
    return malloc(sizeof(Beastiary));
}
