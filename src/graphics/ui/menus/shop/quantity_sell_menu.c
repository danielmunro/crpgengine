#include "headers/graphics/ui/menu.h"

int getQuantitySellCursorLength() {
    return 2;
}

void drawQuantitySellMenuScreen(MenuContext *mc) {
    TextBox *b = findOrCreateTextBox(
            mc,
            QUANTITY_BUY_BOX,
            mc->context->ui->textAreas->alert);
    drawMenuRect(mc->context->ui->menu, b->area);
    drawInMenu(b, "How many would you like to sell?");
    char amount[24];
    sprintf(amount, "%s x %d", mc->itemToSell.item->name, mc->quantity);
    drawInMenu(b, amount);
    TextBox *b2 = findOrCreateTextBox(
            mc,
            SHOP_QUANTITY_SELL_BOX,
            mc->context->ui->textAreas->alertRight);
    char costText[24];
    sprintf(costText, "= %d", (mc->quantity) * mc->itemToSell.item->worth);
    drawInMenu(b2, "");
    drawInMenu(b2, costText);
    drawScrollableInMenuWithStyle(
            b,
            mc->cursorLine == 1 ? mc->fonts->highlight : mc->fonts->default_,
            "                 cancel",
            0,
            2);
    drawScrollableInMenuWithStyle(
            b,
            mc->cursorLine == 0 ? mc->fonts->highlight : mc->fonts->default_,
            "confirm",
            0,
            2);
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    b->area.x + ((float) mc->cursorLine * 125),
                    b->area.y + line(2, mc->fonts->default_->lineHeight),
            });
}

MenuSelectResponse *quantitySellMenuItemSelected(const MenuContext *mc) {
    return createMenuSelectResponse(
            RESPONSE_TYPE_OPEN_MENU,
            SHOP_CONFIRM_SALE_MENU);
}

int getMaxSellQuantity(const MenuContext *mc) {
    return mc->itemToSell.amount;
}

MenuKeyPressedType quantitySellKeyPressed(const MenuContext *mc) {
    if (IsKeyPressed(KEY_UP)) {
        if (getMaxSellQuantity(mc) > mc->quantity) {
            return KEY_PRESSED_INCREMENT_QUANTITY;
        }
        return KEY_PRESSED_NOTHING_TO_DO;
    } else if (IsKeyPressed(KEY_DOWN)) {
        if (mc->quantity > 1) {
            return KEY_PRESSED_DECREMENT_QUANTITY;
        }
        return KEY_PRESSED_NOTHING_TO_DO;
    } else if (IsKeyPressed(KEY_LEFT)) {
        return KEY_PRESSED_DECREMENT_CURSOR;
    } else if (IsKeyPressed(KEY_RIGHT)) {
        return KEY_PRESSED_INCREMENT_CURSOR;
    } else if (IsKeyPressed(KEY_SPACE) && mc->cursorLine == 1) {
        return KEY_PRESSED_CLOSE_MENU;
    }
    return menuKeyPressed(mc);
}
