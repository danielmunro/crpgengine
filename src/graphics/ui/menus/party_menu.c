TextBox *createFullScreenTextBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->full,
            mc->fonts->default_,
            IN_GAME_MENU_BOX);
}

TextBox *createPartyTextBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->right,
            mc->fonts->default_,
            PARTY_BOX);
}

void drawPartyMenuScreen(MenuContext *menuContext) {
    TextBox *inGameMenuBox = findOrCreateTextBox(
            menuContext,
            IN_GAME_MENU_BOX,
            createFullScreenTextBox);
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
    }
    int count = sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
    TextBox *textBox = findOrCreateTextBox(
            menuContext,
            PARTY_BOX,
            createPartyTextBox);
    drawMenuRect(textBox->area);
    for (int i = 0; i < count; i++) {
        drawInMenu(textBox, PartyMenuItems[i]);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    (float) ui->screen->width - PARTY_MENU_WIDTH,
                    line(menuContext->cursorLine, defaultFont->lineHeight)
            });
}

int getPartyMenuCursorLength(MenuContext *menuContext) {
    return sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
}

MenuSelectResponse *partyMenuItemSelected(MenuContext *menuContext) {
    int c = menuContext->cursorLine;
    if (strcmp(PartyMenuItems[c], PARTY_MENU_ITEMS) == 0) {
        return createMenuSelectResponse(OPEN_MENU, ITEMS_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_MAGIC) == 0) {
        return createMenuSelectResponse(OPEN_MENU, PARTY_SELECT_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_SAVE) == 0) {
        save(menuContext->player, menuContext->scene);
        return createMenuSelectResponse(OPEN_MENU, ACKNOWLEDGE_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_QUIT) == 0) {
        return createMenuSelectResponse(OPEN_MENU, QUIT_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_LOAD) == 0) {
        return createMenuSelectResponse(OPEN_MENU, LOAD_MENU);
    }
    fprintf(stderr, "menu type not found :: %d", c);
    exit(EXIT_MENU_NOT_DEFINED);
}
