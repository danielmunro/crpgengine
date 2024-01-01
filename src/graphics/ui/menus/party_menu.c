#include "headers/graphics/ui/menu.h"

void drawPartyMenuScreen(MenuContext *mc) {
    const UIConfig *ui = mc->context->ui;
    TextBox *inGameMenuBox = findOrCreateTextBox(
            mc,
            IN_GAME_MENU_BOX,
            ui->textAreas->full);
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
    }
    int count = sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
    TextBox *textBox = findOrCreateTextBox(
            mc,
            PARTY_BOX,
            ui->textAreas->right);
    drawMenuRect(textBox->area);
    for (int i = 0; i < count; i++) {
        if (i == mc->cursorLine) {
            drawInMenuWithStyle(textBox, mc->fonts->highlight, PartyMenuItems[i]);
        } else {
            drawInMenu(textBox, PartyMenuItems[i]);
        }
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    (float) mc->context->user->resolution.width
                            - PARTY_MENU_WIDTH,
                    line(mc->cursorLine, defaultFont->lineHeight)
            });
}

int getPartyMenuCursorLength() {
    return sizeof(PartyMenuItems) / sizeof(PartyMenuItems[0]);
}

MenuSelectResponse *partyMenuItemSelected(const MenuContext *mc) {
    int c = mc->cursorLine;
    if (strcmp(PartyMenuItems[c], PARTY_MENU_ITEMS) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, ITEMS_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_MAGIC) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, PARTY_SELECT_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_SAVE) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_SAVE_GAME, ACKNOWLEDGE_SAVE_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_QUIT) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, QUIT_GAME_MENU);
    } else if (strcmp(PartyMenuItems[c], PARTY_MENU_LOAD) == 0) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, LOAD_SAVE_MENU);
    }
    addError("menu type not found :: %d", c);
    exit(GameEngineErrorMenuNotDefined);
}
