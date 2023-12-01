#include "headers/graphics/ui/menu.h"

int getShopCursorLength(const MenuContext *mc) {
    return sizeof(mc->player->shop->items) / sizeof(ItemWithMarkup);
}

void drawShopMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            SHOP_BOX,
            ui->textAreas->medium);
    drawMenuRect(b->area);
    for (int i = 0; i < getShopCursorLength(menuContext); i++) {
        drawInMenu(b, menuContext->player->shop->items[i]->item->name);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(4 + menuContext->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *shopMenuItemSelected() {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
