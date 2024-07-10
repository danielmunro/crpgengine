#include "headers/graphics/ui/menu.h"

int getSettingsFullScreenMenuCursorLength() {
    return 2;
}

void drawSettingsFullScreenMenuScreen(MenuContext *mc) {
    const FontStyle *defaultFont = mc->fonts->default_;
    TextBox *headersBox = findOrCreateTextBox(
            mc,
            SETTINGS_NAMES_BOX,
            mc->context->ui->textAreas->medium);
    drawMenuRect(mc->context->ui->menu, headersBox->area);
    drawInMenu(headersBox, "Full screen");
    TextBox *valuesBox = findOrCreateTextBox(
            mc,
            SETTINGS_VALUES_BOX,
            mc->context->ui->textAreas->mediumRight);
    if (mc->context->user->fullScreen) {
        drawInMenuWithStyle(valuesBox, mc->fonts->highlight, "*Yes");
        drawInMenuWithStyle(valuesBox, mc->fonts->disable, "No");
    } else {
        drawInMenuWithStyle(valuesBox, mc->fonts->disable, "Yes");
        drawInMenuWithStyle(valuesBox, mc->fonts->highlight, "*No");
    }
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
