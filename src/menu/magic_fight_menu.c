int getMagicFightCursorLength(MenuContext *menuContext) {
    return menuContext->selectedMob->spellCount;
}

TextBox *createMagicFightMenuTextBox(MenuContext *menuContext) {
    return createTextBox(
            drawBottomRightMenu(),
            menuContext->fonts->default_,
            MAGIC_SELECT_BOX);
}

void drawMagicFightMenuScreen(MenuContext *menuContext) {
    drawBottomRightMenu();
    TextBox *t = findOrCreateTextBox(menuContext, MAGIC_SELECT_BOX, createMagicFightMenuTextBox);
    FontStyle *defaultFont = menuContext->fonts->default_;
    FontStyle *disabledFont = menuContext->fonts->disable;
    Mobile *m = menuContext->selectedMob;
    for (int i = 0; i < m->spellCount; i++) {
        FontStyle *fs = canApplyCost(m, m->spells[i]->cost) ? defaultFont : disabledFont;
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

MenuSelectResponse *magicFightMenuItemSelected(MenuContext *menuContext) {
    Spell *spell = menuContext->selectedMob->spells[menuContext->cursorLine];
    return createMenuSelectResponse(
            OPEN_MENU,
            spell->intent == INTENT_HARM ? BEAST_TARGET_FIGHT_MENU : MOBILE_TARGET_FIGHT_MENU);
}
