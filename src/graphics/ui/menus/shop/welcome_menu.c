#include "headers/graphics/ui/menu.h"

int getShopWelcomeCursorLength() {
    return 3;
}

void drawShopWelcomeMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            mc,
            SHOP_WELCOME_BOX,
            mc->context->ui->textAreas->small);
    drawMenuRect(mc->context->ui->menu, b->area);
    drawInMenu(b, "Welcome! What would you like to do?");
    drawInMenu(b, "");
    drawInMenu(b, "Buy");
    drawInMenu(b, "Sell");
    drawInMenu(b, "Exit");
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(mc->cursorLine + 2, defaultFont->lineHeight)
            });
}

MenuSelectResponse *shopWelcomeMenuItemSelected(const MenuContext *mc) {
    if (mc->cursorLine == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, SHOP_BUY_MENU);
    } else if (mc->cursorLine == 1) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, SHOP_SELL_MENU);
    } else if (mc->cursorLine == 2) {
        return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SHOP_WELCOME_MENU);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SHOP_WELCOME_MENU);
}
