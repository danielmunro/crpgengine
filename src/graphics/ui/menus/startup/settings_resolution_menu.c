#include "headers/graphics/ui/menu.h"

int getSettingsResolutionMenuCursorLength() {
    return 2;
}

void drawSettingsResolutionMenuScreen(MenuContext *mc) {
    TextBox *headersBox = findOrCreateTextBox(
            mc,
            SETTINGS_NAMES_BOX,
            mc->context->ui->textAreas->medium);
    drawMenuRect(mc->context->ui->menu, headersBox->area);
    drawInMenu(headersBox, "Resolution");
    TextBox *valuesBox = findOrCreateTextBox(
            mc,
            SETTINGS_VALUES_BOX,
            mc->context->ui->textAreas->mediumRight);
    if (mc->context->user->resolution.width == 800) {
        drawInMenuWithStyle(valuesBox, mc->fonts->highlight, "*800x600");
    } else {
        drawInMenuWithStyle(valuesBox, mc->fonts->disable, "800x600");
    }
    if (mc->context->user->resolution.width == 1280) {
        drawInMenuWithStyle(valuesBox, mc->fonts->highlight, "*1280x1024");
    } else {
        drawInMenuWithStyle(valuesBox, mc->fonts->disable, "1280x1024");
    }
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    valuesBox->area.x,
                    valuesBox->area.y + line(mc->cursorLine, mc->fonts->default_->lineHeight)
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
