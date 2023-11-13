#include "headers/item.h"

typedef struct {
    int id;
    int coins;
    ItemWithMarkup **items;
} Shop;

Shop *createShop(int id, ItemWithMarkup **items, int coins) {
    Shop *s = malloc(sizeof(Shop));
    s->id = id;
    s->items = items;
    s->coins = coins;
    return s;
}
