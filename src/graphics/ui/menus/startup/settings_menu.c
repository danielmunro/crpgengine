#include "headers/graphics/ui/menu.h"

int getSettingsMenuCursorLength() {
    return 1;
}

void drawSettingsMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            SETTINGS_BOX,
            ui->textAreas->medium);
    drawMenuRect(b->area);
    drawInMenu(b, "Hello world");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(2 + menuContext->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *settingsMenuItemSelected() {
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SETTINGS_MENU);
}
