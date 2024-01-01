#include "headers/graphics/ui/menu.h"

int getCannotAffordCursorLength() {
    return 0;
}

void drawCannotAffordMenuScreen(MenuContext *mc) {
    TextBox *b = findOrCreateTextBox(
            mc,
            CANNOT_AFFORD_BOX,
            mc->context->ui->textAreas->alert);
    drawMenuRect(b->area);
    drawInMenu(b, "Sorry, you cannot afford that.");
}

MenuSelectResponse *cannotAffordMenuItemSelected() {
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SHOP_CANNOT_AFFORD_MENU);
}
