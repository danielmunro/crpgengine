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

void drawFullScreenOptions(TextBox *tb, const MenuContext *mc) {
    if (mc->userConfig->fullScreen) {
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*Yes");
        drawInMenuWithStyle(tb, mc->fonts->disable, "No");
    } else {
        drawInMenuWithStyle(tb, mc->fonts->disable, "Yes");
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*No");
    }
}

void drawDifficultyOptions(TextBox *tb, const MenuContext *mc) {
    if (mc->userConfig->difficulty == DIFFICULTY_CASUAL) {
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*Casual");
        drawInMenuWithStyle(tb, mc->fonts->disable, "Normal");
        drawInMenuWithStyle(tb, mc->fonts->disable, "Challenge");
    } else if (mc->userConfig->difficulty == DIFFICULTY_NORMAL) {
        drawInMenuWithStyle(tb, mc->fonts->disable, "Casual");
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*Normal");
        drawInMenuWithStyle(tb, mc->fonts->disable, "Challenge");
    } else {
        drawInMenuWithStyle(tb, mc->fonts->disable, "Casual");
        drawInMenuWithStyle(tb, mc->fonts->disable, "Normal");
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*Challenge");
    }
}

void drawResolutionOptions(TextBox *tb, const MenuContext *mc) {
    if (mc->userConfig->resolution.width == 800) {
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*800x600");
        drawInMenuWithStyle(tb, mc->fonts->disable, "1200x800");
    } else {
        drawInMenuWithStyle(tb, mc->fonts->disable, "800x600");
        drawInMenuWithStyle(tb, mc->fonts->highlight, "*1200x800");
    }
}
