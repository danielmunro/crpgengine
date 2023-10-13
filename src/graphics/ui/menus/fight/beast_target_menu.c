#include "headers/graphics/ui/menus/fight/beast_list_menu.h"

int geBeastTargetMenuCursorLength(const MenuContext *menuContext) {
    return getBeastSelectCursorLength(menuContext);
}

void drawBeastTargetMenuScreen(MenuContext *menuContext) {
    drawBeastSelectMenuScreen(menuContext);
    TextBox *t = findOrCreateTextBox(
            menuContext,
            BEAST_SELECT_BOX,
            ui->textAreas->bottomLeft);
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    menuContext->fonts->default_->lineHeight *
                    (float) menuContext->cursorLine,
            });
    if (menuContext->fight->beastCount <= menuContext->cursorLine) {
        return;
    }
    Rectangle p = menuContext->fight->beasts[menuContext->cursorLine]->position;
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    p.x - (p.width / 2),
                    p.y - (p.height / 2),
            });
}

MenuSelectResponse *beastTargetMenuItemSelected(MenuContext *menuContext) {
    menuContext->targetBeast = menuContext->fight->beasts[menuContext->cursorLine];
    return beastSelectMenuItemSelected(menuContext);
}
