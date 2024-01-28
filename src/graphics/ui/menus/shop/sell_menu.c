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
    if (mc->itemListCount == 0) {
        ItemListResult result = createItemList(mc->player);
        mc->itemList = result.itemList;
        mc->itemListCount = result.count;
    }
    for (int i = 0; i < mc->itemListCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", mc->itemList[i].amount, mc->itemList[i].item->name);
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
    mc->itemToSell = mc->itemList[mc->cursorLine];
    if (mc->itemList[mc->cursorLine].amount == 1) {
        for (int i = 0; i < mc->player->itemCount; i++) {
            if (mc->player->items[i] == mc->itemToSell.item) {
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
