#include "headers/graphics/ui/menu.h"

void drawItemsMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            menuContext,
            ITEMS_BOX,
            ui->textAreas->full);
    drawMenuRect(textBox->area);
    if (menuContext->itemListCount == 0) {
        ItemListResult result = createItemList(menuContext->player);
        menuContext->itemList = result.itemList;
        menuContext->itemListCount = result.count;
    }
    for (int i = 0; i < menuContext->itemListCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", menuContext->itemList[i].amount, menuContext->itemList[i].item->name);
        drawInMenuWithStyle(
                textBox,
                menuContext->itemList[i].item->type == ITEM_TYPE_CONSUMABLE
                ? menuContext->fonts->default_
                : menuContext->fonts->disable,
                buffer);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    0,
                    line(menuContext->cursorLine, defaultFont->lineHeight),
            });
}

int getItemsCursorLength(const MenuContext *menuContext) {
    return menuContext->itemListCount;
}

MenuSelectResponse *itemMenuItemSelected(MenuContext *menuContext) {
    Item *item = menuContext->itemList[menuContext->cursorLine].item;
    if (item->type == ITEM_TYPE_CONSUMABLE) {
        menuContext->selectedItem = item;
        return createMenuSelectResponse(OPEN_MENU, PARTY_APPLY_MENU);
    }
    return NULL;
}
