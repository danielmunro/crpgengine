#include "headers/graphics/ui/menu.h"

int getMagicSelectCursorLength(const MenuContext *menuContext) {
    return menuContext->selectedMob->spellCount;
}

void drawMagicSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MAGIC_SELECT_BOX,
            ui->textAreas->bottomMidRight);
    drawMenuRect(t->area);
    FontStyle *defaultFont = menuContext->fonts->default_;
    FontStyle *disabledFont = menuContext->fonts->disable;
    const Mobile *m = menuContext->selectedMob;
    for (int i = 0; i < m->spellCount; i++) {
        FontStyle *fs = canApplyCost(m, menuContext->spells[i]->cost) ? defaultFont : disabledFont;
        drawInMenuWithStyle(t, fs, Spells[m->spells[i]->type]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    defaultFont->lineHeight *
                    (float) menuContext->cursorLine,
            });
}

MenuSelectResponse *magicSelectMenuItemSelected(MenuContext *menuContext) {
    menuContext->selectedSpell = menuContext->selectedMob->spells[menuContext->cursorLine];
    return createMenuSelectResponse(
            RESPONSE_TYPE_OPEN_MENU,
            menuContext->selectedSpell->intent == INTENT_HARM
                    ? BEAST_TARGET_FIGHT_MENU
                    : MOBILE_TARGET_FIGHT_MENU);
}
