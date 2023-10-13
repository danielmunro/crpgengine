#include "headers/graphics/ui/menu.h"

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

int getActionSelectCursorLength() {
    return sizeof(actionMenu) / sizeof(char *);
}

void drawActionSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            ACTION_SELECT_BOX,
            ui->textAreas->bottomMid);
    drawMenuRect(t->area);
    drawScrollableInMenu(t, actionMenu[ACTION_ATTACK], menuContext->cursorLine);
    drawScrollableInMenu(
            t,
            menuContext->selectedMob->spellCount > 0
            ? actionMenu[ACTION_MAGIC] : "",
            menuContext->cursorLine);
    drawScrollableInMenu(t, actionMenu[ACTION_ITEMS], menuContext->cursorLine);
    drawScrollableInMenu(t, actionMenu[ACTION_DEFEND], menuContext->cursorLine);
    drawScrollableInMenu(t, actionMenu[ACTION_RUN], menuContext->cursorLine);
    float h = menuContext->fonts->default_->lineHeight;
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y
                    + (h * (float) menuContext->cursorLine)
                    - getScrollOffset(h, menuContext->cursorLine, t->area.height),
            });
}

bool canGoToIndex(const Mobile *mob, int index) {
    if (index == ACTION_MAGIC) {
        return mob->spellCount > 0;
    }
    return true;
}

int getNextActionSelectCursorPosition(const MenuContext *mc) {
    for (int i = mc->cursorLine + 1; i < getActionSelectCursorLength(); i++) {
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

int getPreviousActionSelectCursorPosition(const MenuContext *mc) {
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
        menuContext->actionType = ATTACK;
        return createMenuSelectResponse(OPEN_MENU, BEAST_TARGET_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_MAGIC) {
        menuContext->actionType = CAST;
        return createMenuSelectResponse(OPEN_MENU, MAGIC_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_ITEMS) {
        menuContext->actionType = ITEM;
        return createMenuSelectResponse(OPEN_MENU, ITEMS_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_DEFEND) {
        return createMenuSelectResponse(DEFEND_SELECTED, ACTION_SELECT_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_RUN) {
        return createMenuSelectResponse(RESPONSE_TYPE_RUN, ACTION_SELECT_FIGHT_MENU);
    }
    return createMenuSelectResponse(CLOSE_MENU, ACTION_SELECT_FIGHT_MENU);
}
