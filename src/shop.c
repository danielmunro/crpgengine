#include "headers/item.h"

typedef struct {
    int coins;
    ItemWithMarkup **items;
} Shop;

Shop *createShop(ItemWithMarkup **items, int coins) {
    Shop *s = malloc(sizeof(Shop));
    s->items = items;
    s->coins = coins;
    return s;
}
