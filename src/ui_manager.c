typedef struct {
    Font font;
} UIManager;

UIManager *createUIManager(const char *font) {
    UIManager *ui = malloc(sizeof(UIManager));
    ui->font = LoadFont(font);
    return ui;
}
