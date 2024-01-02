#include "headers/graphics/ui/menu.h"

int getShopBuyCursorLength(const MenuContext *mc) {
    return mc->shop->itemCount;
}

void drawShopBuyMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            mc,
            SHOP_BUY_BOX,
            mc->context->ui->textAreas->medium);
    TextBox *b2 = findOrCreateTextBox(
            mc,
            SHOP_COSTS_BOX,
            mc->context->ui->textAreas->mediumRight);
    drawMenuRect(mc->context->ui->menu, b->area);
    char coinsAmount[64];
    sprintf(coinsAmount, "You have %d coins", mc->player->coins);
    drawInMenu(b, coinsAmount);
    drawInMenu(b, "");
    drawInMenu(b2, "");
    drawInMenu(b2, "");
    for (int i = 0; i < getShopBuyCursorLength(mc); i++) {
        const ItemWithMarkup *item = mc->shop->items[i];
        const FontStyle *fs;
        if (item->worth > mc->player->coins) {
            fs = mc->fonts->disable;
        } else if (mc->cursorLine == i) {
            fs = mc->fonts->highlight;
        } else {
            fs = mc->fonts->default_;
        }
        drawInMenuWithStyle(b, fs, item->item->name);
        char cost[4];
        sprintf(cost, "%d", item->worth);
        drawInMenuWithStyle(b2, fs, cost);
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(mc->cursorLine + 2, defaultFont->lineHeight)
            });
}

MenuSelectResponse *shopBuyMenuItemSelected(MenuContext *mc) {
    ItemWithMarkup *item = mc->shop->items[mc->cursorLine];
    if (mc->player->coins < item->worth) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, SHOP_CANNOT_AFFORD_MENU);
    }
    mc->itemToBuy = item;
    return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, SHOP_QUANTITY_SELECT_MENU);
}
