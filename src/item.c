typedef struct {
    int hp;
    int mp;
} ConsumeAffect;

typedef struct {
    ItemType type;
    const char *id;
    const char *name;
    Attributes *attributes;
    ConsumeAffect *consumeAffect;
} Item;

Item *createItem(
        ItemType type,
        const char *id,
        const char *name,
        Attributes *attributes,
        ConsumeAffect *consumeAffect) {
    Item *item = malloc(sizeof(Item));
    item->type = type;
    item->id = &id[0];
    item->name = &name[0];
    item->attributes = attributes;
    item->consumeAffect = consumeAffect;
    return item;
}
