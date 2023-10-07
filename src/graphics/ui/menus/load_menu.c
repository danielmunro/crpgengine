#include "src/graphics/ui/menu.h"

int getLoadCursorLength(const MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

void drawLoadMenuScreen(MenuContext *menuContext) {
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            LOAD_BOX,
            ui->textAreas->small);
    for (int i = 0; i < menuContext->saveFiles->count; i++) {
        drawInMenu(b, menuContext->saveFiles->saveNames[i]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y
                    + ui->menu->padding
                    + line(menuContext->cursorLine, defaultFont->lineHeight),
            });
    free(b);
}

MenuSelectResponse *loadMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, LOAD_MENU);
}
