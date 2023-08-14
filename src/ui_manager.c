typedef struct {
    Font font;
    Menu *menus[MAX_MENUS];
    int menuCount;
} UIManager;

UIManager *createUIManager(const char *indexDir, const char *font) {
    UIManager *ui = malloc(sizeof(UIManager));
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/fonts/%s", indexDir, font);
    ui->font = LoadFont(path);
    ui->menuCount = getMenuList(ui->menus);
    return ui;
}

Menu *findMenu(UIManager *ui, MenuType type) {
    for (int i = 0; i < ui->menuCount; i++) {
        if (ui->menus[i]->type == type) {
            return ui->menus[i];
        }
    }
    return NULL;
}
