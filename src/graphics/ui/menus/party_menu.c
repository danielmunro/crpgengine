#include "headers/graphics/ui/menu.h"

void drawPartyMenuScreen(MenuContext *menuContext) {
    TextBox *inGameMenuBox = findOrCreateTextBox(
            menuContext,
            IN_GAME_MENU_BOX,
            ui->textAreas->full);
    drawMenuRect(inGameMenuBox->area);
    FontStyle *defaultFont = menuContext->fonts->default_;
    for (int i = 0; i < menuContext->player->partyCount; i++) {
        Mobile *mob = menuContext->player->party[i];
        float column1 = getAvatarWidth(mob->avatar) + (ui->menu->padding * 2);
        float y = ui->menu->padding + ((float) ui->screen->height / 4) * (float) i;
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
    }
    int count = sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
    TextBox *textBox = findOrCreateTextBox(
            menuContext,
            PARTY_BOX,
            ui->textAreas->right);
    drawMenuRect(textBox->area);
    for (int i = 0; i < count; i++) {
        if (i == menuContext->cursorLine) {
            drawInMenuWithStyle(textBox, menuContext->fonts->highlight, PartyMenuItems[i]);
        } else {
            drawInMenu(textBox, PartyMenuItems[i]);
        }
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    (float) ui->screen->width
                            - PARTY_MENU_WIDTH,
                    line(menuContext->cursorLine, defaultFont->lineHeight)
            });
}

int getPartyMenuCursorLength() {
    return sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
}

MenuSelectResponse *partyMenuItemSelected(const MenuContext *menuContext) {
    int c = menuContext->cursorLine;
    if (strcmp(PartyMenuItems[c], PARTY_MENU_ITEMS) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, ITEMS_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_MAGIC) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, PARTY_SELECT_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_SAVE) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_SAVE_GAME, ACKNOWLEDGE_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_QUIT) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, QUIT_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_LOAD) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, LOAD_MENU);
    }
    addError("menu type not found :: %d", c);
    exit(GameEngineErrorMenuNotDefined);
}
