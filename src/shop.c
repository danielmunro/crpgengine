#include "headers/item.h"

typedef struct {
    const char *welcome;
    const char *goodbye;
} Messages;

typedef struct {
    int id;
    int coins;
    ItemWithMarkup **items;
    int itemCount;
    Messages *messages;
} Shop;

Messages *createMessagesFromData(const MessageData *data) {
    Messages *m = malloc(sizeof(Messages));
    m->welcome = data->welcome;
    m->goodbye = data->goodbye;
    return m;
}

Shop *createShop(int id, ItemWithMarkup **items, int itemCount, int coins, Messages *messages) {
    Shop *s = malloc(sizeof(Shop));
    s->id = id;
    s->items = items;
    s->itemCount = itemCount;
    s->coins = coins;
    s->messages = messages;
    return s;
}
