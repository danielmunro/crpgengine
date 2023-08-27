int getMobileTargetMenuCursorLength(MenuContext *menuContext) {
    return getMobileSelectMenuCursorLength(menuContext);
}

void drawMobileTargetMenuScreen(MenuContext *menuContext) {
    drawBottomRightMenu();
    TextBox *t = findOrCreateTextBox(menuContext, MOBILE_SELECT_BOX, createMobileSelectTextBox);
    drawPlayerFightTopLevel(menuContext, t, false);
    Vector2 playerPosition = getFightPlayerPosition(menuContext->cursorLine);
    int frameWidth = menuContext->selectedMob->animations[0]->spriteSheet->frameWidth;
    int frameHeight = menuContext->selectedMob->animations[0]->spriteSheet->frameHeight;
    drawRightCursor(
            menuContext->fight->menuSprite,
            (Vector2) {
                    playerPosition.x - (float) frameWidth,
                    (float) (playerPosition.y - frameHeight / 1.5),
            });
}

MenuSelectResponse *mobileTargetMenuItemSelected(MenuContext *menuContext) {
    return beastSelectMenuItemSelected(menuContext);
}
