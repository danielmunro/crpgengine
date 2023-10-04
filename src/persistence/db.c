void purgeSaves(const char *indexDir) {
    const char *saveDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) saveDirectory, "%s/_saves", indexDir);
    char **files = calloc(MAX_SAVE_FILES, sizeof(char *));
    int count = getFilesInDirectory(saveDirectory, files);
    for (int i = 0; i < count; i++) {
        printf("remove save file: %s\n", files[i]);
        char *filepath = malloc(MAX_FS_PATH_LENGTH);
        sprintf(filepath, "%s/%s", saveDirectory, files[i]);
        remove(filepath);
        free(filepath);
    }
    free(files);
}
