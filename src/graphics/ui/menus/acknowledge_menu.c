#include "src/headers/menu_type.h"
#include "src/graphics/ui/menu.h"

int getAcknowledgeCursorLength(const MenuContext *menuContext) {
    return 1;
}

void drawAcknowledgeMenuScreen(MenuContext *menuContext) {
    const FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *b = findOrCreateTextBox(
            menuContext,
            ACKNOWLEDGE_BOX,
            ui->textAreas->medium);
    drawMenuRect(b->area);
    drawInMenu(b, "Your game has been saved.");
    drawInMenu(b, "Reminder: your progress will save");
    drawInMenu(b, "automatically.");
    drawInMenu(b, "");
    drawInMenu(b, "ok");
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    b->area.x,
                    b->area.y + line(4 + menuContext->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *acknowledgeMenuItemSelected(const MenuContext *menuContext) {
    return createMenuSelectResponse(CLOSE_MENU, SAVE_MENU);
}
