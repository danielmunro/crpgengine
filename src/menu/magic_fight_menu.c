int getMagicFightCursorLength(MenuContext *menuContext) {
    return menuContext->selectedMob->spellCount;
}

TextBox *createMagicFightMenuTextBox(MenuContext *menuContext) {
    return createTextBox(
            drawBottomRightMenu(),
            getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT),
            MAGIC_SELECT_BOX);
}

void drawMagicFightMenuScreen(MenuContext *menuContext) {
    drawBottomRightMenu();
    TextBox *t = findOrCreateTextBox(menuContext, MAGIC_SELECT_BOX, createMagicFightMenuTextBox);
    FontStyle *defaultFont = getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT);
    FontStyle *disabledFont = getFontStyle(menuContext->fonts, FONT_STYLE_DISABLED);
    Mobile *m = menuContext->selectedMob;
    for (int i = 0; i < m->spellCount; i++) {
        FontStyle *fs = m->mana >= m->spells[i]->cost.mana ? defaultFont : disabledFont;
        drawInMenuWithStyle(t, fs, Spells[m->spells[i]->type]);
    }
    drawRightCursor(
            menuContext->fight->menuSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y + (float) (LINE_HEIGHT * menuContext->cursorLine),
            });
}

MenuSelectResponse *magicFightMenuItemSelected(MenuContext *menuContext) {
    Spell *spell = menuContext->selectedMob->spells[menuContext->cursorLine];
    return createMenuSelectResponse(
            OPEN_MENU,
            spell->intent == INTENT_HARM ? BEAST_TARGET_FIGHT_MENU : MOBILE_TARGET_FIGHT_MENU);
}
