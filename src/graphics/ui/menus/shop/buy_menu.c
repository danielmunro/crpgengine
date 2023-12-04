#include "headers/graphics/ui/menu.h"

int getShopBuyCursorLength(const MenuContext *mc) {
    return mc->player->shop->itemCount;
}

void drawShopBuyMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            SHOP_BUY_BOX,
            ui->textAreas->medium);
    drawMenuRect(b->area);
    for (int i = 0; i < getShopBuyCursorLength(menuContext); i++) {
        drawInMenu(b, menuContext->player->shop->items[i]->item->name);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(menuContext->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *shopBuyMenuItemSelected() {
    return createMenuSelectResponse(CLOSE_MENU, SHOP_BUY_MENU);
}
