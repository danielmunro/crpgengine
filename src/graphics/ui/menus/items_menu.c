#include "headers/graphics/ui/menu.h"

void drawItemsMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            mc,
            ITEMS_BOX,
            mc->context->ui->textAreas->full);
    drawMenuRect(mc->context->ui->menu, textBox->area);
    if (mc->items == NULL) {
        mc->items = mc->player->items;
        mc->itemListCount = mc->player->itemCount;
    }
    for (int i = 0; i < mc->itemListCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", mc->items[i]->quantity, mc->items[i]->name);
        drawInMenuWithStyle(
                textBox,
                mc->items[i]->type == ITEM_TYPE_CONSUMABLE
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
    Item *item = menuContext->items[menuContext->cursorLine];
    if (item->type == ITEM_TYPE_CONSUMABLE) {
        menuContext->selectedItem = item;
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, PARTY_APPLY_MENU);
    }
    return NULL;
}

void unloadItemsMenu(MenuContext *mc) {
    mc->items = NULL;
    mc->itemListCount = 0;
}
