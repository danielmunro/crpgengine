#include "headers/graphics/ui/menu.h"

int getLoadCursorLength(const MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

void drawLoadMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            LOAD_BOX,
            ui->textAreas->small);
    drawMenuRect(b->area);
    for (int i = 0; i < menuContext->saveFiles->count; i++) {
        drawInMenu(b, menuContext->saveFiles->saves[i]->saveName);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y
                    + line(menuContext->cursorLine, defaultFont->lineHeight),
            });
}

MenuSelectResponse *loadMenuItemSelected() {
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, LOAD_MENU);
}
