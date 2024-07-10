#include "headers/graphics/ui/menu.h"

int getSettingsMenuCursorLength() {
    return 3;
}

void drawSettingsMenuScreen(MenuContext *mc) {
    TextBox *headersBox = findOrCreateTextBox(
            mc,
            SETTINGS_NAMES_BOX,
            mc->context->ui->textAreas->medium);
    drawMenuRect(mc->context->ui->menu, headersBox->area);
    drawInMenu(headersBox, "Full screen");
    drawInMenu(headersBox, "Difficulty");
    drawInMenu(headersBox, "Resolution");
    TextBox *valuesBox = findOrCreateTextBox(
            mc,
            SETTINGS_VALUES_BOX,
            mc->context->ui->textAreas->mediumRight);
    if (mc->context->user->fullScreen) {
        drawInMenu(valuesBox, "Yes");
    } else {
        drawInMenu(valuesBox, "No");
    }
    if (mc->context->user->difficulty == DIFFICULTY_CASUAL) {
        drawInMenu(valuesBox, "Casual");
    } else if (mc->context->user->difficulty == DIFFICULTY_NORMAL) {
        drawInMenu(valuesBox, "Normal");
    } else {
        drawInMenu(valuesBox, "Challenge");
    }
    if (mc->context->user->resolution.width == 800) {
        drawInMenu(valuesBox, "800x600");
    } else {
        drawInMenu(valuesBox, "1280x1024");
    }
    const FontStyle *defaultFont = mc->fonts->default_;
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    headersBox->area.x,
                    headersBox->area.y + line(mc->cursorLine, defaultFont->lineHeight)
            });
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
