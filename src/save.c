#include "mm_malloc.h"
#include "headers/util/log.h"
#include "headers/persistence/cyaml.h"

typedef struct {
    const char *filename;
    const char *saveName;
    unsigned long created;
} SaveFile;

typedef struct {
    int count;
    const SaveFile **saves;
} SaveFiles;

SaveFile *createSaveFile(const char *filename, const char *saveName, unsigned long created) {
    SaveFile *sf = malloc(sizeof(SaveFile));
    sf->filename = filename;
    sf->saveName = saveName;
    sf->created = created;
    return sf;
}

SaveFiles *createSaveFiles() {
    SaveFiles *sf = malloc(sizeof(SaveFiles));
    sf->saves = calloc(MAX_SAVE_FILES, sizeof(SaveFile));
    sf->count = 0;
    return sf;
}

void saveFile(const SaveData *save, const char *indexDir, const char *filename) {
    char filePathAuto[MAX_FS_PATH_LENGTH];
    sprintf(filePathAuto, "%s/%s/%s", indexDir, "_saves", filename);
    addInfo("player save file :: %s", filePathAuto);
    saveSaveData(save, filePathAuto);
}

void sortSaveFiles(const SaveFiles *sf) {
    for (int i = 0; i < sf->count; i++) {
        for (int j = 0; j < sf->count; j++) {
            if (sf->saves[i]->created > sf->saves[j]->created) {
                const SaveFile *tmp = sf->saves[i];
                sf->saves[i] = sf->saves[j];
                sf->saves[j] = tmp;
            }
        }
    }
}

SaveFiles *getSaveFiles() {
    const char *savesDirectory = malloc(MAX_FS_PATH_LENGTH);
    sprintf((char *) savesDirectory, "%s/_saves", config->indexDir);
    char **files = calloc(MAX_SAVE_FILES, sizeof(char *));
    int count = getFilesInDirectory(savesDirectory, files);
    SaveFiles *sf = createSaveFiles();
    for (int i = 0; i < count; i++) {
        char *filePath = malloc(MAX_FS_PATH_LENGTH);
        sprintf(filePath, "%s/%s", savesDirectory, files[i]);
        SaveData *s = loadSaveData(filePath);
        if (strcmp(files[i], "autosave.yaml") == 0) {
            char *name = malloc(MAX_SAVE_NAME);
            sprintf(name, "(autosave) %s", s->name);
            sf->saves[sf->count] =  createSaveFile(
                    filePath,
                    name,
                    s->time + 1);
            sf->count++;
        } else {
            sf->saves[sf->count] =  createSaveFile(
                    filePath,
                    s->name,
                    s->time);
            sf->count++;
        }
        free(filePath);
        free(s);
    }
    sortSaveFiles(sf);
    free((char *) savesDirectory);
    return sf;
}

void addSaveFile(SaveFiles *sf, const SaveFile *f) {
    sf->saves[sf->count] = f;
    sf->count++;
    sortSaveFiles(sf);
}
