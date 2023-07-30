int getSaveFiles(const char *indexDir, char *saveFiles[MAX_SAVE_FILES]) {
    char *savesDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf(savesDirectory, "%s/_saves", indexDir);
    return getFilesInDirectory(savesDirectory, saveFiles);
}

int getSaveCursorLength(MenuContext *menuContext) {
    return sizeof(QuitMenuItems) / sizeof(QuitMenuItems[0]);
}

void drawSaveMenuScreen(MenuContext *menuContext) {
//    char *saveFiles[MAX_SAVE_FILES];
//    int files = getSaveFiles();
}

MenuSelectResponse *saveMenuItemSelected(MenuType menuType) {
    if (strcmp(QuitMenuItems[menuType], QUIT_MENU_YES) == 0) {
        exit(0);
    }
    return createMenuSelectResponse(CLOSE_MENU, QUIT_MENU);
}
