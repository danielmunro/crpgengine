typedef struct {
    const char *scene;
    Player *player;
    SaveFiles *saveFiles;
    const char *indexDir;
    int cursorLine;
} MenuContext;

typedef struct {
    MenuSelectResponseType type;
    MenuType menuType;
} MenuSelectResponse;

typedef struct {
    MenuType type;
    int cursor;
    int (*getCursorLength)(MenuContext *);
    void (*draw)(MenuContext *);
    MenuSelectResponse *(*selected)(MenuContext *menuContext);
} Menu;

MenuSelectResponse *createMenuSelectResponse(MenuSelectResponseType type, MenuType menuType) {
    MenuSelectResponse *response = malloc(sizeof(MenuSelectResponse));
    response->type = type;
    response->menuType = menuType;
    return response;
}

Menu *createMenu(
        MenuType type,
        int (getCursorLength)(MenuContext *),
        void (draw)(MenuContext *),
        MenuSelectResponse *(*selected)()) {
    Menu *menu = malloc(sizeof(Menu));
    menu->cursor = 0;
    menu->type = type;
    menu->getCursorLength = getCursorLength;
    menu->draw = draw;
    menu->selected = selected;
    return menu;
}

SaveFiles *getSaveFiles(const char *indexDir) {
    const char *savesDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *)savesDirectory, "%s/_saves", indexDir);
    char *files[MAX_SAVE_FILES];
    const char **names = calloc(MAX_SAVE_FILES, MAX_FS_PATH_LENGTH);
    unsigned long created[MAX_SAVE_FILES];
    int count = getFilesInDirectory(savesDirectory, files);
    for (int i = 0; i < count; i++) {
        char *filePath = malloc(MAX_FS_PATH_LENGTH);
        sprintf(filePath, "%s/%s", savesDirectory, files[i]);
        SaveData *s = loadSaveData(filePath);
        if (strcmp(files[i], "autosave.yaml") == 0) {
            char *name = malloc(MAX_SAVE_NAME);
            sprintf(name, "(autosave) %s", s->name);
            names[i] = name;
            created[i] = s->time + 1;
        } else {
            names[i] = s->name;
            created[i] = s->time;
        }
    }
    for (int i = 0; i < count; i++) {
        for (int j = 0; j < count; j++) {
            if (created[i] > created[j]) {
                char *s = &files[i][0];
                files[i] = files[j];
                files[j] = &s[0];
                unsigned long c = created[i];
                created[i] = created[j];
                created[j] = c;
                const char *n = &names[i][0];
                names[i] = names[j];
                names[j] = n;
            }
        }
    }
    SaveFiles *sf = malloc(sizeof(SaveFiles));
    sf->count = count;
    sf->filenames = (const char **)files;
    sf->saveNames = names;
    return sf;
}

MenuContext *createMenuContext(
        Player *player,
        const char *scene,
        const char *indexDir,
        int cursorLine) {
    MenuContext *context = malloc(sizeof(MenuContext));
    context->player = player;
    context->scene = scene;
    context->indexDir = indexDir;
    context->cursorLine = cursorLine;
    context->saveFiles = getSaveFiles(indexDir);
    return context;
}

Menu *findMenu(Menu *menus[MAX_MENUS], int menuCount, MenuType type) {
    for (int i = 0; i < menuCount; i++) {
        if (menus[i]->type == type) {
            return menus[i];
        }
    }
    return NULL;
}

void drawAllMenus(
        Player *player,
        Menu *menus[MAX_MENUS],
        int menuCount,
        const char *scene,
        const char *indexDir) {
    BeginDrawing();
    for (int i = 0; i < menuCount; i++) {
        menus[i]->draw(
                createMenuContext(player, scene, indexDir, menus[i]->cursor));
    }
    EndDrawing();
}

void normalizeMenuCursor(Menu *menu, MenuContext *menuContext) {
    if (menu->cursor >= menu->getCursorLength(menuContext)) {
        menu->cursor = 0;
    }

    if (menu->cursor < 0) {
        menu->cursor = menu->getCursorLength(menuContext) - 1;
    }
}
