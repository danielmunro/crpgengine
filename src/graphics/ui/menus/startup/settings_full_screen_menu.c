#include "headers/graphics/ui/menu.h"
#include "headers/graphics/ui/menus/startup/settings_common.h"

int getSettingsFullScreenMenuCursorLength() {
    return 2;
}

void drawSettingsFullScreenMenuScreen(MenuContext *mc) {
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
                    valuesBox->area.y + line(mc->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *settingsFullScreenMenuItemSelected(const MenuContext *mc) {
    UserConfig *userConfig = mc->context->user;
    if (mc->cursorLine == 0 && !userConfig->fullScreen) {
        ToggleFullscreen();
        userConfig->fullScreen = true;
        saveUserConfig(userConfig, mc->context->indexDir);
    } else if (mc->cursorLine == 1 && userConfig->fullScreen) {
        ToggleFullscreen();
        userConfig->fullScreen = false;
        saveUserConfig(userConfig, mc->context->indexDir);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SETTINGS_FULL_SCREEN_MENU);
}
