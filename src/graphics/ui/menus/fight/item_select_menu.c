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
    if (mc->items == NULL) {
        mc->items = mc->player->items;
        mc->itemListCount = mc->player->itemCount;
    }
    for (int i = 0; i < mc->itemListCount; i++) {
        const Item *item = mc->items[i];
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", mc->items[i]->quantity, item->name);
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
    Item *item = menuContext->items[menuContext->cursorLine];
    if (item->type != ITEM_TYPE_CONSUMABLE) {
        return createMenuSelectResponse(RESPONSE_TYPE_NONE, ITEMS_MENU);
    }
    menuContext->selectedItem = item;
    menuContext->actionType = ITEM;
    return createMenuSelectResponse(
            RESPONSE_TYPE_OPEN_MENU,
            MOBILE_TARGET_FIGHT_MENU);
}

void unloadItemSelectMenu(MenuContext *mc) {
    mc->items = NULL;
    mc->itemListCount = 0;
}
