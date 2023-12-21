#include "headers/graphics/ui/ui.h"
#include "headers/graphics/ui/menu.h"

void drawSettingsHeader(MenuContext *mc) {
    TextBox *headersBox = findOrCreateTextBox(
            mc,
            SETTINGS_NAMES_BOX,
            ui->textAreas->medium);
    drawMenuRect(headersBox->area);
    drawInMenu(headersBox, "Full screen");
    drawInMenu(headersBox, "");
    drawInMenu(headersBox, "Difficulty");
    drawInMenu(headersBox, "");
    drawInMenu(headersBox, "");
    drawInMenu(headersBox, "Resolution");

    int offset = 0;
    if (mc->cursorLine == 1) {
        offset = 1;
    } else if (mc->cursorLine == 2) {
        offset = 3;
    }
    const FontStyle *defaultFont = mc->fonts->default_;
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    headersBox->area.x,
                    headersBox->area.y + line(offset + mc->cursorLine, defaultFont->lineHeight)
            });
}

void drawFullScreenOption(TextBox *tb, const MenuContext *mc) {
    if (mc->userConfig->fullScreen) {
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*Yes");
    } else {
        drawInMenuWithStyle(tb, mc->fonts->default_, "Yes");
    }
    if (mc->userConfig->fullScreen) {
        drawInMenuWithStyle(tb, mc->fonts->default_, "No");
    } else {
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*No");
    }
}