#include "headers/graphics/ui/menu.h"
#include "headers/graphics/ui/menus/startup/settings_common.h"

int getSettingsMenuCursorLength() {
    return 3;
}

void drawSettingsMenuScreen(MenuContext *mc) {
    drawSettingsHeader(mc);
    TextBox *valuesBox = findOrCreateTextBox(
            mc,
            SETTINGS_VALUES_BOX,
            ui->textAreas->mediumRight);
    drawFullScreenOptions(valuesBox, mc);
    drawDifficultyOptions(valuesBox, mc);
    drawResolutionOptions(valuesBox, mc);
}

MenuSelectResponse *settingsMenuItemSelected(const MenuContext *mc) {
    addInfo("cursorLine %d", mc->cursorLine);
    if (mc->cursorLine == 0) {
        return createMenuSelectResponse(
                RESPONSE_TYPE_OPEN_MENU,
                SETTINGS_FULL_SCREEN_MENU);
    } else if (mc->cursorLine == 1) {
        return createMenuSelectResponse(
                RESPONSE_TYPE_OPEN_MENU,
                SETTINGS_DIFFICULTY_MENU);
    } else if (mc->cursorLine == 2) {
        return createMenuSelectResponse(
                RESPONSE_TYPE_OPEN_MENU,
                SETTINGS_RESOLUTION_MENU);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_NONE, SETTINGS_MENU);
}
