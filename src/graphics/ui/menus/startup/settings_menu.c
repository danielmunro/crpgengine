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
    const FontStyle *highlightFont = mc->fonts->highlight;
    const FontStyle *disableFont = mc->fonts->disable;
    drawFullScreenOption(valuesBox, mc);
    drawInMenuWithStyle(valuesBox, disableFont, "Casual");
    drawInMenuWithStyle(valuesBox, highlightFont, "Normal");
    drawInMenuWithStyle(valuesBox, disableFont, "Challenge");
    drawInMenuWithStyle(valuesBox, highlightFont, "800x600");
}

MenuSelectResponse *settingsMenuItemSelected(const MenuContext *mc) {
    if (mc->cursorLine == 0) {
        return createMenuSelectResponse(
                RESPONSE_TYPE_OPEN_MENU,
                SETTINGS_FULL_SCREEN_MENU);
    } else if (mc->cursorLine == 2) {
        return createMenuSelectResponse(
                RESPONSE_TYPE_OPEN_MENU,
                SETTINGS_DIFFICULTY_MENU);
    } else if (mc->cursorLine == 3) {
        return createMenuSelectResponse(
                RESPONSE_TYPE_OPEN_MENU,
                SETTINGS_RESOLUTION_MENU);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_NONE, SETTINGS_MENU);
}
