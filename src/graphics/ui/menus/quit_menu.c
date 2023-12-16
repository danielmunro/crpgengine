#include "headers/graphics/ui/menu.h"

int getQuitCursorLength() {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

void drawQuitMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            menuContext,
            QUIT_BOX,
            ui->textAreas->medium);
    drawMenuRect(textBox->area);
    drawInMenu(textBox, "Are you sure?");
    drawInMenu(textBox, "");
    int cursorLength = getQuitCursorLength();
    for (int i = 0; i < cursorLength; i++) {
        drawInMenu(textBox, QuitMenuItems[i]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    textBox->area.x,
                    textBox->area.y +
                    line(cursorLength + menuContext->cursorLine, defaultFont->lineHeight),
            });
}

MenuSelectResponse *quitMenuItemSelected(const MenuContext *menuContext) {
    if (strcmp(QuitMenuItems[menuContext->cursorLine], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, QUIT_GAME_MENU);
}
