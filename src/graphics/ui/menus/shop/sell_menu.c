#include "headers/graphics/ui/menu.h"

int getShopSellCursorLength(const MenuContext *mc) {
    return mc->player->itemCount;
}

void drawShopSellMenuScreen(MenuContext *mc) {
    TextBox *contentBox = findOrCreateTextBox(
            mc,
            SHOP_SELL_BOX,
            mc->context->ui->textAreas->medium);
    drawMenuRect(mc->context->ui->menu, contentBox->area);
    for (int i = 0; i < mc->player->itemCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", mc->player->items[i]->quantity, mc->player->items[i]->name);
        drawScrollableInMenuWithStyle(
                contentBox,
                mc->fonts->default_,
                buffer,
                mc->cursorLine,
                i);
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                contentBox->area.x,
                contentBox->area.y
                + line(mc->cursorLine, mc->fonts->default_->lineHeight)
                - getScrollOffset(mc->fonts->default_->lineHeight, mc->cursorLine, contentBox->area.height),
            });
}

MenuSelectResponse *shopSellMenuItemSelected(MenuContext *mc) {
    if (mc->player->items[mc->cursorLine]->quantity == 1) {
        for (int i = 0; i < mc->player->itemCount; i++) {
            if (strcmp(mc->player->items[i]->name, mc->player->items[mc->cursorLine]->name) == 0) {
                return createMenuSelectResponse(
                        RESPONSE_TYPE_OPEN_MENU,
                        SHOP_CONFIRM_SALE_MENU);
            }
        }
    }
    return createMenuSelectResponse(
            RESPONSE_TYPE_OPEN_MENU,
            SHOP_QUANTITY_SELL_MENU);
}

void unloadItemSellMenu(MenuContext *mc) {
}
