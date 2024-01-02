#include "headers/graphics/ui/menu.h"

void drawItemsMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            mc,
            ITEMS_BOX,
            mc->context->ui->textAreas->full);
    drawMenuRect(mc->context->ui->menu, textBox->area);
    if (mc->itemListCount == 0) {
        ItemListResult result = createItemList(mc->player);
        mc->itemList = result.itemList;
        mc->itemListCount = result.count;
    }
    for (int i = 0; i < mc->itemListCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", mc->itemList[i].amount, mc->itemList[i].item->name);
        drawInMenuWithStyle(
                textBox,
                mc->itemList[i].item->type == ITEM_TYPE_CONSUMABLE
                ? mc->fonts->default_
                : mc->fonts->disable,
                buffer);
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    0,
                    line(mc->cursorLine, defaultFont->lineHeight),
            });
}

int getItemsCursorLength(const MenuContext *menuContext) {
    return menuContext->itemListCount;
}

MenuSelectResponse *itemMenuItemSelected(MenuContext *menuContext) {
    Item *item = menuContext->itemList[menuContext->cursorLine].item;
    if (item->type == ITEM_TYPE_CONSUMABLE) {
        menuContext->selectedItem = item;
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, PARTY_APPLY_MENU);
    }
    return NULL;
}
