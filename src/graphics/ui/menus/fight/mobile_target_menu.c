#include "../../../../headers/menu.h"
#include "mobile_select_menu.h"

int getMobileTargetMenuCursorLength(MenuContext *menuContext) {
    return getMobileSelectMenuCursorLength(menuContext);
}

void drawMobileTargetMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MOBILE_SELECT_BOX,
            ui->textAreas->bottomMidRight);
    drawMenuRect(t->area);
    drawPlayerFightTopLevel(menuContext, t, false);
    int frameWidth = menuContext->selectedMob->animations[0]->spriteSheet->frameWidth;
    int frameHeight = menuContext->selectedMob->animations[0]->spriteSheet->frameHeight;
    Vector2 playerPosition = getFightPlayerPosition(
            menuContext->cursorLine,
            frameHeight);
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    playerPosition.x - (float) frameWidth,
                    (float) (playerPosition.y - frameHeight / 1.5),
            });
}

MenuSelectResponse *mobileTargetMenuItemSelected(MenuContext *menuContext) {
    menuContext->targetMob = menuContext->player->party[menuContext->cursorLine];
    return createMenuSelectResponse(FIND_TARGET_MENU, MOBILE_TARGET_FIGHT_MENU);
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
