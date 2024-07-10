#include "headers/graphics/ui/menu.h"

int getSettingsDifficultyMenuCursorLength() {
    return 3;
}

void drawSettingsDifficultyMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *headersBox = findOrCreateTextBox(
            mc,
            SETTINGS_NAMES_BOX,
            mc->context->ui->textAreas->medium);
    drawMenuRect(mc->context->ui->menu, headersBox->area);
    drawInMenu(headersBox, "Difficulty");
    TextBox *valuesBox = findOrCreateTextBox(
            mc,
            SETTINGS_VALUES_BOX,
            mc->context->ui->textAreas->mediumRight);
    if (mc->context->user->difficulty == DIFFICULTY_CASUAL) {
        drawInMenuWithStyle(valuesBox, mc->fonts->highlight, "*Casual");
    } else {
        drawInMenuWithStyle(valuesBox, mc->fonts->disable, "Casual");
    }
    if (mc->context->user->difficulty == DIFFICULTY_NORMAL) {
        drawInMenuWithStyle(valuesBox, mc->fonts->highlight, "*Normal");
    } else {
        drawInMenuWithStyle(valuesBox, mc->fonts->disable, "Normal");
    }
    if (mc->context->user->difficulty == DIFFICULTY_CHALLENGE) {
        drawInMenuWithStyle(valuesBox, mc->fonts->highlight, "*Challenge");
    } else {
        drawInMenuWithStyle(valuesBox, mc->fonts->disable, "Challenge");
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    valuesBox->area.x,
                    valuesBox->area.y + line(mc->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *settingsDifficultyMenuItemSelected(const MenuContext *mc) {
    UserConfig *userConfig = mc->context->user;
    if (mc->cursorLine == 0) {
        userConfig->difficulty = DIFFICULTY_CASUAL;
    } else if (mc->cursorLine == 1) {
        userConfig->difficulty = DIFFICULTY_NORMAL;
    } else if (mc->cursorLine == 2) {
        userConfig->difficulty = DIFFICULTY_CHALLENGE;
    }
    saveUserConfig(userConfig, mc->context->indexDir);
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SETTINGS_DIFFICULTY_MENU);
}
