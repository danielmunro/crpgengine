#include "headers/action_type.h"
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

void drawActionSelectMenuScreen(MenuContext *mc) {
    TextBox *t = findOrCreateTextBox(
            mc,
            ACTION_SELECT_BOX,
            mc->context->ui->textAreas->bottomMid);
    drawMenuRect(mc->context->ui->menu, t->area);
    drawScrollableInMenu(t, actionMenu[ACTION_ATTACK], mc->cursorLine);
    drawScrollableInMenu(
            t,
            mc->selectedMob->spellCount > 0
            ? actionMenu[ACTION_MAGIC] : "",
            mc->cursorLine);
    drawScrollableInMenu(t, actionMenu[ACTION_ITEMS], mc->cursorLine);
    drawScrollableInMenu(t, actionMenu[ACTION_DEFEND], mc->cursorLine);
    drawScrollableInMenu(t, actionMenu[ACTION_RUN], mc->cursorLine);
    float h = mc->fonts->default_->lineHeight;
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    t->area.x,
                    t->area.y
                    + (h * (float) mc->cursorLine)
                    - getScrollOffset(h, mc->cursorLine, t->area.height),
            });
}

bool canGoToIndex(const Mobile *mob, int index) {
    if (index == ACTION_MAGIC) {
        return mob->spellCount > 0;
    }
    return true;
}

int getNextActionSelectCursorPosition(const MenuContext *mc, const int maxCursorLine) {
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

int getPreviousActionSelectCursorPosition(const MenuContext *mc, const int maxCursorLine) {
    for (int i = mc->cursorLine - 1; i >= 0; i--) {
        if (canGoToIndex(mc->selectedMob, i)) {
            return i;
        }
    }
    for (int i = getActionSelectCursorLength() - 1; i > mc->cursorLine; i--) {
        if (canGoToIndex(mc->selectedMob, i)) {
            return i;
        }
    }
    return mc->cursorLine;
}

MenuSelectResponse *actionSelectMenuItemSelected(MenuContext *menuContext) {
    if (menuContext->cursorLine == ACTION_ATTACK) {
        menuContext->actionType = ATTACK;
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, BEAST_TARGET_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_MAGIC) {
        menuContext->actionType = CAST;
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, MAGIC_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_ITEMS) {
        menuContext->actionType = ITEM;
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, ITEMS_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_DEFEND) {
        return createMenuSelectResponse(RESPONSE_TYPE_DEFEND_SELECTED, ACTION_SELECT_FIGHT_MENU);
    } else if (menuContext->cursorLine == ACTION_RUN) {
        return createMenuSelectResponse(RESPONSE_TYPE_RUN, ACTION_SELECT_FIGHT_MENU);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, ACTION_SELECT_FIGHT_MENU);
}
