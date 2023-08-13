void drawPlayer(Player *player) {
    drawAnimation(
            findAnimation(getPartyLeader(player)->animations, DOWN),
            (Vector2) {20, 20}
    );
}

void drawPartyMenuScreen(MenuContext *menuContext) {
    drawFullscreenMenu();
    drawPlayer(menuContext->player);
    int column1 = (UI_PADDING * 2) + MOB_COLLISION_WIDTH;
    int column2 = SCREEN_WIDTH - 200;
    drawText(getPartyLeader(menuContext->player)->name, (Vector2D) {column1, UI_PADDING});
    char hp[64];
    sprintf(hp, "hp %d/%d", 20, 20);
    drawText(hp, (Vector2D) {column1, UI_PADDING + line(1)});
    char mp[64];
    sprintf(mp, "mp %d/%d", 20, 20);
    drawText(mp, (Vector2D) {column1, UI_PADDING + line(2)});
    int count = sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
    TextBox *textBox = createTextBox((Rectangle) {
            (float) column2,
            0,
            200,
            SCREEN_HEIGHT - (UI_PADDING * 2)
    });
    for (int i = 0; i < count; i++) {
        drawInMenu(textBox, PartyMenuItems[i]);
    }
    drawText(">", (Vector2D) {column2 - 20, UI_PADDING + line(menuContext->cursorLine)});
    free(textBox);
}

int getPartyMenuCursorLength(MenuContext *menuContext) {
    return sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
}


MenuSelectResponse *partyMenuItemSelected(MenuContext *menuContext) {
    int c = menuContext->cursorLine;
    if (strcmp(PartyMenuItems[c], PARTY_MENU_ITEMS) == 0) {
        return createMenuSelectResponse(OPEN_MENU, ITEMS_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_SAVE) == 0) {
        save(menuContext->player, menuContext->scene, menuContext->indexDir);
        return createMenuSelectResponse(OPEN_MENU, ACKNOWLEDGE_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_QUIT) == 0) {
        return createMenuSelectResponse(OPEN_MENU, QUIT_MENU);
    }  else if (strcmp(PartyMenuItems[c], PARTY_MENU_LOAD) == 0) {
        return createMenuSelectResponse(OPEN_MENU, LOAD_MENU);
    }
    fprintf(stderr, "menu type not found :: %d", c);
    exit(EXIT_MENU_NOT_DEFINED);
}
