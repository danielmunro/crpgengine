#include "headers/graphics/ui/menu.h"

int getQuantitySelectCursorLength() {
    return 2;
}

void drawQuantitySelectMenuScreen(MenuContext *mc) {
    TextBox *b = findOrCreateTextBox(
            mc,
            QUANTITY_SELECT_BOX,
            ui->textAreas->alert);
    drawMenuRect(b->area);
    drawInMenu(b, "How many would you like to buy?");
    char amount[24];
    sprintf(amount, "%s x %d", mc->itemToBuy->item->name, mc->quantity);
    drawInMenu(b, amount);
    TextBox *b2 = findOrCreateTextBox(
            mc,
            SHOP_QUANTITY_COSTS_BOX,
            ui->textAreas->alertRight);
    char costText[24];
    sprintf(costText, "= %d", (mc->quantity) * mc->itemToBuy->worth);
    drawInMenu(b2, "");
    drawInMenu(b2, costText);
    drawScrollableInMenuWithStyle(b, mc->cursorLine == 1 ? mc->fonts->highlight : mc->fonts->default_, "                 cancel", 3);
    drawScrollableInMenuWithStyle(b, mc->cursorLine == 0 ? mc->fonts->highlight : mc->fonts->default_, "confirm", 3);
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    b->area.x + ((float) mc->cursorLine * 125),
                    b->area.y + line(2, mc->fonts->default_->lineHeight),
            });
}

MenuSelectResponse *quantitySelectMenuItemSelected(const MenuContext *mc) {
    if (mc->cursorLine == 1) {
        return createMenuSelectResponse(CLOSE_MENU, SHOP_QUANTITY_SELECT_MENU);
    }
    return createMenuSelectResponse(OPEN_MENU, SHOP_CONFIRM_PURCHASE_MENU);
}

int getMaxQuantity(const MenuContext *mc) {
    return mc->player->coins / mc->itemToBuy->worth;
}

MenuKeyPressedResponse *quantitySelectKeyPressed(const MenuContext *mc) {
    if (IsKeyPressed(KEY_UP)) {
        if (getMaxQuantity(mc) > mc->quantity) {
            return createMenuKeyPressedResponse(KEY_PRESSED_INCREMENT_QUANTITY);
        }
        return createMenuKeyPressedResponse(KEY_PRESSED_NOTHING_TO_DO);
    } else if (IsKeyPressed(KEY_DOWN)) {
        if (mc->quantity > 1) {
            return createMenuKeyPressedResponse(KEY_PRESSED_DECREMENT_QUANTITY);
        }
        return createMenuKeyPressedResponse(KEY_PRESSED_NOTHING_TO_DO);
    } else if (IsKeyPressed(KEY_LEFT)) {
        return createMenuKeyPressedResponse(KEY_PRESSED_DECREMENT_CURSOR);
    } else if (IsKeyPressed(KEY_RIGHT)) {
        return createMenuKeyPressedResponse(KEY_PRESSED_INCREMENT_CURSOR);
    }
    return menuKeyPressed();
}
