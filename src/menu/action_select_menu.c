const char *actionMenu[] = {
        "Attack",
        "Magic",
        "Items",
        "Defend",
        "Run",
};

typedef enum {
    ATTACK,
    MAGIC,
    ITEMS,
    DEFEND,
    RUN,
} ActionMenu;

int getActionSelectCursorLength(MenuContext *menuContext) {
    return sizeof(actionMenu) / sizeof(char *);
}

TextBox *createActionSelectMenuTextBox(MenuContext *menuContext) {
    return createTextBox(
            getActionSelectMenu(),
            menuContext->fonts->default_,
            ACTION_SELECT_BOX);
}

void drawActionSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            ACTION_SELECT_BOX,
            createActionSelectMenuTextBox);
    drawMenuRect(t->area);
    drawInMenu(t, actionMenu[ATTACK]);
    drawInMenu(
            t,
            menuContext->selectedMob->spellCount > 0
            ? actionMenu[MAGIC] : "");
    drawInMenu(t, actionMenu[ITEMS]);
    drawInMenu(t, actionMenu[DEFEND]);
    drawInMenu(t, actionMenu[RUN]);
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    menuContext->fonts->default_->lineHeight *
                    (float) menuContext->cursorLine,
            });
}

MenuSelectResponse *actionSelectMenuItemSelected(MenuContext *menuContext) {
    if (menuContext->cursorLine == ATTACK) {
        return createMenuSelectResponse(OPEN_MENU, BEAST_TARGET_FIGHT_MENU);
    } else if (menuContext->cursorLine == MAGIC) {
        return createMenuSelectResponse(OPEN_MENU, MAGIC_FIGHT_MENU);
    } else if (menuContext->cursorLine == ITEMS) {
        return createMenuSelectResponse(OPEN_MENU, ITEMS_FIGHT_MENU);
    }
    return createMenuSelectResponse(CLOSE_MENU, ACTION_SELECT_FIGHT_MENU);
}

bool canGoToIndex(Mobile *mob, int index) {
    if (index == MAGIC) {
        return mob->spellCount > 0;
    }
    return true;
}

int getNextActionSelectCursorPosition(MenuContext *mc) {
    for (int i = mc->cursorLine + 1; i < getActionSelectCursorLength(mc); i++) {
        if (canGoToIndex(mc->selectedMob, i)) {
            return i;
        }
    }
    for (int i = 0; i < mc->cursorLine; i++) {
        if (canGoToIndex(mc->selectedMob, i)) {
            return i;
        }
    }
    return mc->cursorLine;
}

int getPreviousActionSelectCursorPosition(MenuContext *mc) {
    for (int i = mc->cursorLine - 1; i >= 0; i--) {
        if (canGoToIndex(mc->selectedMob, i)) {
            return i;
        }
    }
    for (int i = getActionSelectCursorLength(mc) - 1; i > mc->cursorLine; i--) {
        if (canGoToIndex(mc->selectedMob, i)) {
            return i;
        }
    }
    return mc->cursorLine;
}

