#include "../../../../headers/menu.h"

int getItemSelectCursorLength(MenuContext *menuContext) {
    return menuContext->player->itemCount;
}

void drawItemSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            ITEM_SELECT_FIGHT_BOX,
            ui->textAreas->bottom);
    drawMenuRect(t->area);
    FontStyle *defaultFont = menuContext->fonts->default_;
    FontStyle *disabledFont = menuContext->fonts->disable;
    ItemListResult result = createItemList(menuContext->player);
    menuContext->itemList = result.itemList;
    menuContext->itemListCount = result.count;
    for (int i = 0; i < result.count; i++) {
        Item *item = result.itemList[i].item;
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", result.itemList[i].amount, item->name);
        FontStyle *fs = item->type == ITEM_TYPE_CONSUMABLE ? defaultFont : disabledFont;
        drawInMenuWithStyle(t, fs, buffer);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    defaultFont->lineHeight *
                    (float) menuContext->cursorLine,
            });
}

MenuSelectResponse *itemSelectMenuItemSelected(MenuContext *menuContext) {
    Item *item = menuContext->itemList[menuContext->cursorLine].item;
    if (item->type != ITEM_TYPE_CONSUMABLE) {
        return createMenuSelectResponse(NO_OP, ITEMS_MENU);
    }
    menuContext->selectedItem = item;
    menuContext->actionType = ITEM;
    return createMenuSelectResponse(
            OPEN_MENU,
            MOBILE_TARGET_FIGHT_MENU);
}
