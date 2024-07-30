#include "headers/graphics/ui/menu.h"

int getItemSelectCursorLength(const MenuContext *menuContext) {
    return menuContext->player->itemCount;
}

void drawItemSelectMenuScreen(MenuContext *mc) {
    TextBox *t = findOrCreateTextBox(
            mc,
            ITEM_SELECT_FIGHT_BOX,
            mc->context->ui->textAreas->bottom);
    drawMenuRect(mc->context->ui->menu, t->area);
    const FontStyle *defaultFont = mc->fonts->default_;
    const FontStyle *disabledFont = mc->fonts->disable;
    if (mc->itemList == NULL) {
        ItemListResult result = createItemList(mc->player);
        mc->itemList = result.itemList;
        mc->itemListCount = result.count;
    }
    for (int i = 0; i < mc->itemListCount; i++) {
        const Item *item = mc->itemList[i].item;
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", mc->itemList[i].amount, item->name);
        const FontStyle *fs = item->type == ITEM_TYPE_CONSUMABLE ? defaultFont : disabledFont;
        drawInMenuWithStyle(t, fs, buffer);
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    defaultFont->lineHeight *
                    (float) mc->cursorLine,
            });
}

MenuSelectResponse *itemSelectMenuItemSelected(MenuContext *menuContext) {
    Item *item = menuContext->itemList[menuContext->cursorLine].item;
    if (item->type != ITEM_TYPE_CONSUMABLE) {
        return createMenuSelectResponse(RESPONSE_TYPE_NONE, ITEMS_MENU);
    }
    menuContext->selectedItem = item;
    menuContext->actionType = ITEM;
    return createMenuSelectResponse(
            RESPONSE_TYPE_OPEN_MENU,
            MOBILE_TARGET_FIGHT_MENU);
}
