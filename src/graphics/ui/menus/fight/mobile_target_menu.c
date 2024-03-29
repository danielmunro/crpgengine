#include "headers/graphics/ui/menu.h"
#include "headers/graphics/ui/menus/fight/mobile_select_menu.h"

int getMobileTargetMenuCursorLength(const MenuContext *menuContext) {
    return getMobileSelectMenuCursorLength(menuContext);
}

void drawMobileTargetMenuScreen(MenuContext *mc) {
    TextBox *t = findOrCreateTextBox(
            mc,
            MOBILE_SELECT_BOX,
            mc->context->ui->textAreas->bottomMidRight);
    drawMenuRect(mc->context->ui->menu, t->area);
    drawPlayerFightTopLevel(mc, t, false);
    int frameWidth = mc->selectedMob->animations[0]->spriteSheet->frameWidth;
    int frameHeight = mc->selectedMob->animations[0]->spriteSheet->frameHeight;
    Vector2 playerPosition = getFightPlayerPosition(
            mc->cursorLine,
            frameHeight,
            mc->context->user->resolution);
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    playerPosition.x - (float) frameWidth,
                    (float) (playerPosition.y - frameHeight / 1.5),
            });
}

MenuSelectResponse *mobileTargetMenuItemSelected(MenuContext *menuContext) {
    menuContext->targetMob = menuContext->player->party[menuContext->cursorLine];
    return createMenuSelectResponse(RESPONSE_TYPE_FIND_TARGET_MENU, MOBILE_TARGET_FIGHT_MENU);
}

int getNextMobileTargetCursorPosition(const MenuContext *mc, const int maxCursorLine) {
    const Player *p = mc->player;
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

int getPreviousMobileTargetCursorPosition(const MenuContext *mc, const int maxCursorLine) {
    const Player *p = mc->player;
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
