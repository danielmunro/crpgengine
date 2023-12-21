#include "headers/graphics/ui/menu.h"

int getSettingsFullScreenMenuCursorLength() {
    return 2;
}

void drawSettingsFullScreenMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    const FontStyle *disableFont = mc->fonts->disable;
    const FontStyle *highlightFont = mc->fonts->highlight;
    TextBox *valuesBox = findOrCreateTextBox(
            mc,
            SETTINGS_VALUES_BOX,
            ui->textAreas->mediumRight);
    if (mc->userConfig->fullScreen) {
        drawInMenuWithStyle(valuesBox, highlightFont, "*Yes");
    } else {
        drawInMenuWithStyle(valuesBox, defaultFont, "Yes");
    }
    if (mc->userConfig->fullScreen) {
        drawInMenuWithStyle(valuesBox, defaultFont, "No");
    } else {
        drawInMenuWithStyle(valuesBox, highlightFont, "*No");
    }
    drawInMenuWithStyle(valuesBox, disableFont, "Casual");
    drawInMenuWithStyle(valuesBox, disableFont, "Normal");
    drawInMenuWithStyle(valuesBox, disableFont, "Challenge");
    drawInMenuWithStyle(valuesBox, disableFont, "800x600");
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    valuesBox->area.x,
                    valuesBox->area.y + line(mc->cursorLine, defaultFont->lineHeight)
            });
}

MenuSelectResponse *settingsFullScreenMenuItemSelected(const MenuContext *mc) {
    addInfo("cursorLine %d", mc->cursorLine);
    if (mc->cursorLine == 0 && !mc->userConfig->fullScreen) {
        ToggleFullscreen();
        mc->userConfig->fullScreen = true;
        saveUserConfig(mc->userConfig);
    } else if (mc->cursorLine == 1 && mc->userConfig->fullScreen) {
        ToggleFullscreen();
        mc->userConfig->fullScreen = false;
        saveUserConfig(mc->userConfig);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_CLOSE_MENU, SETTINGS_FULL_SCREEN_MENU);
}
