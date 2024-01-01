#include "headers/graphics/ui/menu.h"
#include "headers/graphics/ui/menus/startup/settings_common.h"

int getSettingsDifficultyMenuCursorLength() {
    return 3;
}

void drawSettingsDifficultyMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *valuesBox = findOrCreateTextBox(
            mc,
            SETTINGS_VALUES_BOX,
            mc->context->ui->textAreas->mediumRight);
    drawFullScreenOptions(valuesBox, mc);
    drawDifficultyOptions(valuesBox, mc);
    drawResolutionOptions(valuesBox, mc);
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    valuesBox->area.x,
                    valuesBox->area.y + line(mc->cursorLine + 2, defaultFont->lineHeight)
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
    saveUserConfig(userConfig);
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SETTINGS_DIFFICULTY_MENU);
}
