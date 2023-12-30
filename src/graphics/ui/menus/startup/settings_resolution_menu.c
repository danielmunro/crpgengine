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
            ui->textAreas->mediumRight);
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
    addInfo("resolution cursor %d", mc->cursorLine);
    if (mc->cursorLine == 0) {
        mc->userConfig->resolution = (Resolution) { 800, 600 };
    } else if (mc->cursorLine == 1) {
        mc->userConfig->resolution = (Resolution) { 1280, 1024 };
    }
    SetWindowSize(mc->userConfig->resolution.width, mc->userConfig->resolution.height);
    setTextAreasFromData(mc->userConfig->resolution);
    clearTextBoxes(mc->textBoxes);
    saveUserConfig(mc->userConfig);
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SETTINGS_RESOLUTION_MENU);
}
