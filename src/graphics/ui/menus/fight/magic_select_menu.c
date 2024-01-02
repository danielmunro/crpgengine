#include "headers/graphics/ui/menu.h"

int getMagicSelectCursorLength(const MenuContext *menuContext) {
    return menuContext->selectedMob->spellCount;
}

void drawMagicSelectMenuScreen(MenuContext *mc) {
    TextBox *t = findOrCreateTextBox(
            mc,
            MAGIC_SELECT_BOX,
            mc->context->ui->textAreas->bottomMidRight);
    drawMenuRect(mc->context->ui->menu, t->area);
    const FontStyle *defaultFont = mc->fonts->default_;
    const FontStyle *disabledFont = mc->fonts->disable;
    const Mobile *m = mc->selectedMob;
    for (int i = 0; i < m->spellCount; i++) {
        const FontStyle *fs = canApplyCost(m, mc->spells[i]->cost) ? defaultFont : disabledFont;
        drawInMenuWithStyle(t, fs, Spells[m->spells[i]->type]);
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    defaultFont->lineHeight *
                    (float) mc->cursorLine,
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
