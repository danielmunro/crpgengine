const char *actionMenu[] = {
        "Attack",
        "Magic",
        "Items",
        "Defend",
        "Run",
};

typedef enum {
    ACTION_ATTACK,
    ACTION_MAGIC,
    ACTION_ITEMS,
    ACTION_DEFEND,
    ACTION_RUN,
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
    drawInMenu(t, actionMenu[ACTION_ATTACK]);
    drawInMenu(
            t,
            menuContext->selectedMob->spellCount > 0
            ? actionMenu[ACTION_MAGIC] : "");
    drawInMenu(t, actionMenu[ACTION_ITEMS]);
    drawInMenu(t, actionMenu[ACTION_DEFEND]);
    drawInMenu(t, actionMenu[ACTION_RUN]);
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y +
                    menuContext->fonts->default_->lineHeight *
                    (float) menuContext->cursorLine,
            });
}

bool canGoToIndex(Mobile *mob, int index) {
    if (index == ACTION_MAGIC) {
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

MenuSelectResponse *actionSelectMenuItemSelected(MenuContext *menuContext) {
    if (menuContext->cursorLine == ACTION_ATTACK) {
        return createMenuSelectResponse(OPEN_MENU, BEAST_TARGET_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_MAGIC) {
        return createMenuSelectResponse(OPEN_MENU, MAGIC_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_ITEMS) {
        return createMenuSelectResponse(OPEN_MENU, ITEMS_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_DEFEND) {
        return createMenuSelectResponse(DEFEND_SELECTED, ACTION_SELECT_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_RUN) {
        cancelFight(menuContext->fight);
    }
    return createMenuSelectResponse(CLOSE_MENU, ACTION_SELECT_FIGHT_MENU);
}
