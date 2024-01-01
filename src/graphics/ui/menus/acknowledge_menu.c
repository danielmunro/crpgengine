#include "headers/graphics/ui/menu.h"

int getAcknowledgeCursorLength() {
    return 1;
}

void drawAcknowledgeMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            mc,
            ACKNOWLEDGE_BOX,
            mc->context->ui->textAreas->medium);
    drawMenuRect(b->area);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(4 + mc->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *acknowledgeMenuItemSelected() {
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, ACKNOWLEDGE_SAVE_MENU);
}
