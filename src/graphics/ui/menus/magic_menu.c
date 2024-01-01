#include <string.h>
#include <limits.h>
#include "headers/graphics/ui/menu.h"

void drawMagicMenuScreen(MenuContext *mc) {
    TextBox *textBox = findOrCreateTextBox(
            mc,
            MAGIC_BOX,
            mc->context->ui->textAreas->midRight);
    drawMenuRect(textBox->area);
    Mobile *m = mc->selectedMob;
    for (int i = 0; i < m->spellCount; i++) {
        char buffer[MAX_LINE_BUFFER] = "";
        sprintf(buffer, "%s", m->spells[i]->name);
        int count = (int) strlen(m->spells[i]->name) & INT_MAX;
        for (int j = count; j < SPELL_COLUMN_WIDTH; j++) {
            strcat(buffer, " ");
        }
        char spellMana[MAX_VITALS_LENGTH] = "";
        sprintf(spellMana, "%d mp", m->spells[i]->cost->mana);
        strcat(buffer, spellMana);
        FontStyle *fs = mc->fonts->default_;
        if (m->spells[i]->intent != INTENT_HELP || !canApplyCost(m, m->spells[i]->cost)) {
            fs = mc->fonts->disable;
        }
        drawInMenuWithStyle(
                textBox,
                fs,
                buffer);
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    mc->context->ui->textAreas->midRight.x,
                    line(
                            mc->cursorLine,
                            mc->fonts->default_->lineHeight),
            });
}

int getMagicMenuCursorLength(const MenuContext *menuContext) {
    return menuContext->selectedMob->spellCount;
}

MenuSelectResponse *magicMenuItemSelected(MenuContext *menuContext) {
    Spell *spell = menuContext->selectedMob->spells[menuContext->cursorLine];
    if (spell->intent == INTENT_HELP) {
        menuContext->selectedSpell = spell;
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, PARTY_APPLY_MENU);
    }
    return NULL;
}
