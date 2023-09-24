TextBox *createPartySelectTextBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->right,
            mc->fonts->default_,
            PARTY_SELECT_BOX);
}

void drawPartySelectMenuScreen(MenuContext *menuContext) {
    TextBox *inGameMenuBox = findOrCreateTextBox(
            menuContext,
            PARTY_SELECT_BOX,
            createPartySelectTextBox);
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
    return createMenuSelectResponse(CLOSE_MENU, PARTY_SELECT_MENU);
//
//
//    int c = menuContext->cursorLine;
//    if (strcmp(PartyMenuItems[c], PARTY_MENU_ITEMS) == 0) {
//        return createMenuSelectResponse(OPEN_MENU, ITEMS_MENU);
//    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_MAGIC) == 0) {
//        return createMenuSelectResponse(OPEN_MENU, MAGIC_MENU);
//    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_SAVE) == 0) {
//        save(menuContext->player, menuContext->scene);
//        return createMenuSelectResponse(OPEN_MENU, ACKNOWLEDGE_MENU);
//    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_QUIT) == 0) {
//        return createMenuSelectResponse(OPEN_MENU, QUIT_MENU);
//    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_LOAD) == 0) {
//        return createMenuSelectResponse(OPEN_MENU, LOAD_MENU);
//    }
//    fprintf(stderr, "menu type not found :: %d", c);
//    exit(EXIT_MENU_NOT_DEFINED);
}
