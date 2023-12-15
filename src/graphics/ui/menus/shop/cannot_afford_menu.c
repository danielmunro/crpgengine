#include "headers/graphics/ui/menu.h"

int getCannotAffordCursorLength() {
    return 0;
}

void drawCannotAffordMenuScreen(MenuContext *menuContext) {
    TextBox *b = findOrCreateTextBox(
            menuContext,
            CANNOT_AFFORD_BOX,
            ui->textAreas->alert);
    drawMenuRect(b->area);
    drawInMenu(b, "Sorry, you cannot afford that.");
}

MenuSelectResponse *cannotAffordMenuItemSelected() {
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SHOP_CANNOT_AFFORD_MENU);
}
