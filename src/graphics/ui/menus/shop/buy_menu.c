#include "headers/graphics/ui/menu.h"

int getShopBuyCursorLength(const MenuContext *mc) {
    return mc->shop->itemCount;
}

void drawShopBuyMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            SHOP_BUY_BOX,
            ui->textAreas->medium);
    TextBox *b2 = findOrCreateTextBox(
            menuContext,
            SHOP_COSTS_BOX,
            ui->textAreas->mediumRight);
    drawMenuRect(b->area);
    for (int i = 0; i < getShopBuyCursorLength(menuContext); i++) {
        const ItemWithMarkup *item = menuContext->shop->items[i];
        const FontStyle *fs;
        if (item->worth > menuContext->player->coins) {
            fs = menuContext->fonts->disable;
        } else if (menuContext->cursorLine == i) {
            fs = menuContext->fonts->highlight;
        } else {
            fs = menuContext->fonts->default_;
        }
        drawInMenuWithStyle(b, fs, item->item->name);
        char cost[4];
        sprintf(cost, "%d", item->worth);
        drawInMenuWithStyle(b2, fs, cost);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(menuContext->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *shopBuyMenuItemSelected(const MenuContext *mc) {
    const ItemWithMarkup *item = mc->shop->items[mc->cursorLine];
    if (mc->player->coins < item->worth) {
        return createMenuSelectResponse(OPEN_MENU, SHOP_CANNOT_AFFORD_MENU);
    }
    addItem(mc->player, item->item);
    mc->player->coins -= item->worth;
    return createMenuSelectResponse(NO_OP, SHOP_BUY_MENU);
}
