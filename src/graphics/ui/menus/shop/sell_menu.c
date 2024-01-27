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
        drawScrollableInMenuWithStyle(
                contentBox,
                mc->fonts->default_,
                mc->player->items[i]->name,
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

MenuSelectResponse *sellMenuItemSelected() {
    return createMenuSelectResponse(
            RESPONSE_TYPE_CLOSE_MENU,
            SHOP_SELL_MENU);
}

MenuKeyPressedType shopSellMenuKeyPressed(const MenuContext *mc) {
    return menuKeyPressed(mc);
}
