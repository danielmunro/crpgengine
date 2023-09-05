int getMobileTargetMenuCursorLength(MenuContext *menuContext) {
    return getMobileSelectMenuCursorLength(menuContext);
}

void drawMobileTargetMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MOBILE_SELECT_BOX,
            createMobileSelectTextBox);
    drawMenuRect(t->area);
    drawPlayerFightTopLevel(menuContext, t, false);
    Vector2 playerPosition = getFightPlayerPosition(menuContext->cursorLine);
    int frameWidth = menuContext->selectedMob->animations[0]->spriteSheet->frameWidth;
    int frameHeight = menuContext->selectedMob->animations[0]->spriteSheet->frameHeight;
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    playerPosition.x - (float) frameWidth,
                    (float) (playerPosition.y - frameHeight / 1.5),
            });
}

MenuSelectResponse *mobileTargetMenuItemSelected(MenuContext *menuContext) {
    return beastSelectMenuItemSelected(menuContext);
}

int getNextMobileTargetCursorPosition(MenuContext *mc) {
    Player *p = mc->player;
    for (int i = mc->cursorLine + 1; i < mc->player->partyCount; i++) {
        if (p->party[i]->hp > 0) {
            return i;
        }
    }
    for (int i = 0; i < mc->cursorLine; i++) {
        if (p->party[i]->hp > 0) {
            return i;
        }
    }
    return mc->cursorLine;
}

int getPreviousMobileTargetCursorPosition(MenuContext *mc) {
    Player *p = mc->player;
    for (int i = mc->cursorLine - 1; i >= 0; i--) {
        if (p->party[i]->hp > 0) {
            return i;
        }
    }
    for (int i = mc->player->partyCount - 1; i > mc->cursorLine; i--) {
        if (p->party[i]->hp > 0) {
            return i;
        }
    }
    return mc->cursorLine;
}
