void drawPlayer(Player *player) {
    drawAnimation(
            findAnimation(getPartyLeader(player)->animations, DOWN),
            (Vector2) {20, 20}
    );
}

TextBox *createFullScreenTextBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->full,
            mc->fonts->default_,
            IN_GAME_MENU_BOX);
}

TextBox *createPartyTextBox(MenuContext *mc) {
    return createTextBox(
            getPartyMenu(),
            mc->fonts->default_,
            PARTY_BOX);
}

void drawPartyMenuScreen(MenuContext *menuContext) {
    TextBox *inGameMenuBox = findOrCreateTextBox(
            menuContext,
            IN_GAME_MENU_BOX,
            createFullScreenTextBox);
    drawMenuRect(inGameMenuBox->area);
    drawPlayer(menuContext->player);
    FontStyle *defaultFont = menuContext->fonts->default_;
    float column1 = (ui->menu->padding * 2) + MOB_COLLISION_WIDTH;
    drawText(
            getPartyLeader(menuContext->player)->name,
            (Vector2) {column1, ui->menu->padding},
            defaultFont);
    char hp[64];
    sprintf(hp, "hp %d/%d", 20, 20);
    drawText(
            hp,
            (Vector2) {column1, ui->menu->padding + line(1, defaultFont->lineHeight)},
            defaultFont);
    char mp[64];
    sprintf(mp, "mp %d/%d", 20, 20);
    drawText(
            mp,
            (Vector2) {column1, ui->menu->padding + line(2, defaultFont->lineHeight)},
            defaultFont);
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
