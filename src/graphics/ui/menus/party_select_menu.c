void drawPartySelectMenuScreen(MenuContext *menuContext) {
    TextBox *inGameMenuBox = findOrCreateTextBox(
            menuContext,
            PARTY_SELECT_BOX,
            ui->textAreas->left);
    drawMenuRect(inGameMenuBox->area);
    FontStyle *defaultFont = menuContext->fonts->default_;
    for (int i = 0; i < menuContext->player->partyCount; i++) {
        Mobile *mob = menuContext->player->party[i];
        float column1 = (float) mob->avatar->image.width + (ui->menu->padding * 2);
        float y = ui->menu->padding + ((float) ui->screen->height / 4) * (float) i;
        drawAvatar(mob, (Vector2) {ui->menu->padding, y});
        drawText(
                mob->name,
                (Vector2) {column1, y},
                defaultFont);
        Attributes calculated = calculateMobileAttributes(mob);
        drawText(
                getVital(mob->hp, calculated.hp),
                (Vector2) {column1, y + line(1, defaultFont->lineHeight)},
                defaultFont);
        drawText(
                getVital(mob->mana, calculated.mana),
                (Vector2) {column1, y + line(2, defaultFont->lineHeight)},
                defaultFont);
        if (i == menuContext->cursorLine) {
            drawRightCursor(
                    menuContext->uiSprite,
                    (Vector2) {
                            0,
                            (float) (menuContext->cursorLine * i) * ((float) ui->screen->height / 4),
                    });
        }
    }
}

int getPartySelectMenuCursorLength(MenuContext *menuContext) {
    return menuContext->player->partyCount;
}

MenuSelectResponse *partySelectMenuItemSelected(MenuContext *menuContext) {
    menuContext->selectedMob = menuContext->player->party[menuContext->cursorLine];
    return createMenuSelectResponse(PARTY_MEMBER_SELECTED, PARTY_SELECT_MENU);
}
