#include "headers/graphics/ui/menus/fight/beast_list_menu.h"
#include "headers/graphics/ui/menu.h"

int geBeastTargetMenuCursorLength(const MenuContext *menuContext) {
    return getBeastSelectCursorLength(menuContext);
}

void drawBeastTargetMenuScreen(MenuContext *mc) {
    drawBeastSelectMenuScreen(mc);
    TextBox *t = findOrCreateTextBox(
            mc,
            BEAST_SELECT_BOX,
            mc->context->ui->textAreas->bottomLeft);
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    mc->fonts->default_->lineHeight *
                    (float) mc->cursorLine,
            });
    if (mc->fight->beastCount <= mc->cursorLine) {
        return;
    }
    Rectangle p = mc->fight->beasts[mc->cursorLine]->position;
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    p.x - (p.width / 2),
                    p.y - (p.height / 2),
            });
}

MenuSelectResponse *beastTargetMenuItemSelected(MenuContext *menuContext) {
    menuContext->targetBeast = menuContext->fight->beasts[menuContext->cursorLine];
    return beastSelectMenuItemSelected();
}
