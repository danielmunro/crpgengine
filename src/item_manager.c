#include <string.h>

typedef struct {
    Context *context;
    Item **items;
    int count;
} ItemManager;

ItemManager *createItemManager(Context *c) {
    ItemManager *im = malloc(sizeof(ItemManager));
    im->context = c;
    im->items = calloc(MAX_ITEMS, sizeof(Item));
    im->count = 0;
    return im;
}

Item *findItemFromName(ItemManager *im, const char *name) {
    for (int i = 0; i < im->count; i++) {
        if (strcmp(name, im->items[i]->name) == 0) {
            return im->items[i];
        }
    }
    addError("no item found with name provided :: %s", name);
    exit(RuntimeErrorUnknownItem);
}
