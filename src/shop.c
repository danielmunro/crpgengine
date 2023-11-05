typedef struct {
    int coins;
    Item **items;
} Shop;

Shop *createShop(Item **items, int coins) {
    Shop *s = malloc(sizeof(Shop));
    s->items = items;
    s->coins = coins;
    return s;
}
