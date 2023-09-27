void drawMagicMenuScreen(MenuContext *menuContext) {
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            menuContext,
            MAGIC_BOX,
            ui->textAreas->midRight);
    drawMenuRect(textBox->area);
    Mobile *m = menuContext->selectedMob;
    for (int i = 0; i < m->spellCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "%s", m->spells[i]->name);
        int count = strlen(m->spells[i]->name) & INT_MAX;
        for (int j = count; j < 20; j++) {
            sprintf(buffer, "%s ", buffer);
        }
        sprintf(buffer, "%s %dmp", buffer, m->spells[i]->cost->mana);
        FontStyle *fs = menuContext->fonts->default_;
        if (m->spells[i]->intent != INTENT_HELP || !canApplyCost(m, m->spells[i]->cost)) {
            fs = menuContext->fonts->disable;
        }
        drawInMenuWithStyle(
                textBox,
                fs,
                buffer);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    ui->textAreas->midRight.x,
                    line(menuContext->cursorLine, defaultFont->lineHeight),
            });
}

int getMagicMenuCursorLength(MenuContext *menuContext) {
    return menuContext->selectedMob->spellCount;
}

MenuSelectResponse *magicMenuItemSelected(MenuContext *menuContext) {
    Spell *spell = menuContext->selectedMob->spells[menuContext->cursorLine];
    if (spell->intent == INTENT_HELP) {
        menuContext->selectedSpell = spell;
        return createMenuSelectResponse(OPEN_MENU, PARTY_APPLY_MENU);
    }
    return NULL;
}
