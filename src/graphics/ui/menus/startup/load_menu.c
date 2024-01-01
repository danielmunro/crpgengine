#include "headers/graphics/ui/menu.h"

int getLoadCursorLength(const MenuContext *menuContext) {
    return menuContext->saveFiles->count;
}

void drawLoadMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            mc,
            LOAD_BOX,
            mc->context->ui->textAreas->small);
    drawMenuRect(b->area);
    for (int i = 0; i < mc->saveFiles->count; i++) {
        drawInMenu(b, mc->saveFiles->saves[i]->saveName);
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y
                    + line(mc->cursorLine, defaultFont->lineHeight),
            });
}

MenuSelectResponse *loadMenuItemSelected() {
    return createMenuSelectResponse(RESPONSE_TYPE_LOAD_GAME, LOAD_SAVE_MENU);
}
