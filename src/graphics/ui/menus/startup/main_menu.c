#include "headers/graphics/ui/menu.h"

typedef enum {
    MAIN_MENU_NEW_GAME,
    MAIN_MENU_CONTINUE_GAME,
    MAIN_MENU_LOAD_GAME,
    MAIN_MENU_SETTINGS,
    MAIN_MENU_EXIT,
    MAIN_MENU_OPTION_COUNT,
} MainMenuOptions;

int getMainMenuCursorLength() {
    return MAIN_MENU_OPTION_COUNT;
}

bool autosaveExists() {
    char saveFilePath[MAX_FS_PATH_LENGTH];
    strcpy(saveFilePath, getAutosaveFile(config->indexDir));
    return FileExists(saveFilePath);
}

void drawMainMenuScreen(MenuContext *mc) {
    const FontStyle *fs = NULL;
    if (autosaveExists()) {
        fs = mc->fonts->default_;
    } else {
        fs = mc->fonts->disable;
    }
    TextBox *t = findOrCreateTextBox(
            mc,
            MAIN_MENU_BOX,
            mc->context->ui->textAreas->full);
    drawMenuRect(mc->context->ui->menu, t->area);
    drawInMenu(t, "new");
    drawInMenuWithStyle(t, fs, "continue");
    drawInMenu(t, "load");
    drawInMenu(t, "settings");
    drawInMenu(t, "exit");
    drawRightCursor(
            mc->uiSprite,
            (Vector2) {
                    mc->context->ui->textAreas->full.x,
                    line(
                            mc->cursorLine,
                            mc->fonts->default_->lineHeight),
            });
}

int getNextMainMenuCursorPosition(const MenuContext *mc, int maxMenuLength) {
    if (mc->cursorLine == MAIN_MENU_NEW_GAME && !autosaveExists()) {
        return mc->cursorLine + 2;
    }
    return getDefaultNextOption(mc, maxMenuLength);
}

int getPreviousMainMenuCursorPosition(const MenuContext *mc, int maxMenuLength) {
    if (mc->cursorLine == MAIN_MENU_LOAD_GAME && !autosaveExists()) {
        return mc->cursorLine - 2;
    }

    return getDefaultPreviousOption(mc, maxMenuLength);
}

MenuSelectResponse *mainMenuItemSelected(const MenuContext *mc) {
    if (mc->cursorLine == MAIN_MENU_NEW_GAME) {
        return createMenuSelectResponse(RESPONSE_TYPE_NEW_GAME, MAIN_MENU);
    } else if (mc->cursorLine == MAIN_MENU_CONTINUE_GAME) {
        return createMenuSelectResponse(RESPONSE_TYPE_CONTINUE_GAME, MAIN_MENU);
    } else if (mc->cursorLine == MAIN_MENU_LOAD_GAME) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, LOAD_SAVE_MENU);
    } else if (mc->cursorLine == MAIN_MENU_SETTINGS) {
        return createMenuSelectResponse(RESPONSE_TYPE_OPEN_MENU, SETTINGS_MENU);
    } else if (mc->cursorLine == MAIN_MENU_EXIT) {
        return createMenuSelectResponse(RESPONSE_TYPE_EXIT, MAIN_MENU);
    }
    return createMenuSelectResponse(RESPONSE_TYPE_NONE, MAIN_MENU);
}
