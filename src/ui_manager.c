typedef struct {
    Font font;
} UIManager;

UIManager *createUIManager(const char *indexDir, const char *font) {
    UIManager *ui = malloc(sizeof(UIManager));
    char path[MAX_FS_PATH_LENGTH];
    sprintf(path, "%s/fonts/%s", indexDir, font);
    ui->font = LoadFont(path);
    return ui;
}
