#include "headers/graphics/ui/menu.h"

void drawPartySelectMenuScreen(MenuContext *mc) {
    TextBox *inGameMenuBox = findOrCreateTextBox(
            mc,
            PARTY_SELECT_BOX,
            ui->textAreas->left);
    drawMenuRect(inGameMenuBox->area);
    FontStyle *defaultFont = mc->fonts->default_;
    for (int i = 0; i < mc->player->partyCount; i++) {
        Mobile *mob = mc->player->party[i];
        float column1 = getAvatarWidth(mob->avatar) + (ui->menu->padding * 2);
        float y = ui->menu->padding + ((float) mc->context->user->resolution.height / 4) * (float) i;
        drawAvatar(mob->avatar, (Vector2) {ui->menu->padding, y});
        drawText(
                mob->name,
                (Vector2) {column1, y},
                defaultFont);
        Attributes calculated = calculateMobileAttributes(mob);
        char hp[MAX_VITALS_LENGTH];
        getVital(mob->hp, calculated.hp, hp);
        drawText(
                hp,
                (Vector2) {column1, y + line(1, defaultFont->lineHeight)},
                defaultFont);
        char mana[MAX_VITALS_LENGTH];
        getVital(mob->mana, calculated.mana, mana);
        drawText(
                mana,
                (Vector2) {column1, y + line(2, defaultFont->lineHeight)},
                defaultFont);
        if (i == mc->cursorLine) {
            drawRightCursor(
                    mc->uiSprite,
                    (Vector2) {
                            0,
                            (float) (mc->cursorLine * i) * ((float) mc->context->user->resolution.height / 4),
                    });
        }
    }
}

int getPartySelectMenuCursorLength(const MenuContext *menuContext) {
    return menuContext->player->partyCount;
}

MenuSelectResponse *partySelectMenuItemSelected(MenuContext *menuContext) {
    menuContext->selectedMob = menuContext->player->party[menuContext->cursorLine];
    return createMenuSelectResponse(RESPONSE_TYPE_PARTY_MEMBER_SELECTED, PARTY_SELECT_MENU);
}
