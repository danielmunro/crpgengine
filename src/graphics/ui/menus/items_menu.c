#include "headers/graphics/ui/menu.h"

void drawItemsMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            mc,
            ITEMS_BOX,
            mc->context->ui->textAreas->full);
    drawMenuRect(mc->context->ui->menu, textBox->area);
    for (int i = 0; i < mc->player->itemCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", mc->player->items[i]->quantity, mc->player->items[i]->name);
        drawInMenuWithStyle(
                textBox,
                mc->player->items[i]->type == ITEM_TYPE_CONSUMABLE
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
    return menuContext->player->itemCount;
}

MenuSelectResponse *itemMenuItemSelected(MenuContext *menuContext) {
    Item *item = menuContext->player->items[menuContext->cursorLine];
    if (item->type == ITEM_TYPE_CONSUMABLE) {
        menuContext->selectedItem = item;
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, PARTY_APPLY_MENU);
    }
    return NULL;
}

void unloadItemsMenu(MenuContext *mc) {
}
