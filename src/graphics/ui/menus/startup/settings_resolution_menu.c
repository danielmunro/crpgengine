#include "headers/graphics/ui/menu.h"
#include "headers/graphics/ui/menus/startup/settings_common.h"

int getSettingsResolutionMenuCursorLength() {
    return 2;
}

void drawSettingsResolutionMenuScreen(MenuContext *mc) {
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
                    valuesBox->area.y + line(mc->cursorLine + 5, defaultFont->lineHeight)
            });
}

MenuSelectResponse *settingsResolutionMenuItemSelected(const MenuContext *mc) {
    UserConfig *userConfig = mc->context->user;
    if (mc->cursorLine == 0) {
        userConfig->resolution = (Resolution) { 800, 600 };
    } else if (mc->cursorLine == 1) {
        userConfig->resolution = (Resolution) { 1280, 1024 };
    }
    SetWindowSize(userConfig->resolution.width, userConfig->resolution.height);
    setTextAreasFromData(mc->context->ui->textAreas, userConfig->resolution);
    clearTextBoxes(mc->textBoxes);
    saveUserConfig(userConfig, mc->context->indexDir);
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SETTINGS_RESOLUTION_MENU);
}
