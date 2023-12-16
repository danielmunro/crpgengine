#include "mm_malloc.h"
#include "time.h"
#include "headers/util/log.h"
#include "headers/persistence/cyaml.h"
#include "headers/player.h"

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

SaveData *createSaveData(const Player *player, const char *scene, const char *saveName) {
    addDebug("create save data");
    SaveData *save = malloc(sizeof(SaveData));
    save->name = saveName;
    save->player = createPlayerData(player);
    save->scene = scene;
    save->time = (unsigned long) time(NULL);
    return save;
}

SaveFile *save(const Player *player, const char *sceneName) {
    addInfo("save player progress");
    time_t t = time(NULL);
    struct tm result;
    struct tm tm = *localtime_r(&t, &result);
    char *date = malloc(MAX_DATETIME_LENGTH);
    sprintf(date, "%d-%02d-%02d %02d:%02d:%02d", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min,
            tm.tm_sec);
    char *name = malloc(MAX_SAVE_NAME);
    sprintf(name, "%s - %s - %s", date, getPartyLeader(player)->name, sceneName);
    SaveData *save = createSaveData(
            player,
            sceneName,
            name);

    // auto save
    saveFile(save, config->indexDir, "autosave.yaml");
    char filename[MAX_FS_PATH_LENGTH];

    // point-in-time save
    sprintf(filename, "save-%lu.yaml", (unsigned long) time(NULL));
    saveFile(save, config->indexDir, filename);

    SaveFile *s = createSaveFile(filename, save->name, save->time);
    free(date);
    free(save);
    return s;
}

void getSaveFilePathToLoad(char saveFilePath[MAX_FS_PATH_LENGTH]) {
    if (config->saveFile != NULL) {
        sprintf(saveFilePath, "%s/_saves/%s", config->indexDir, config->saveFile);
    } else {
        strcpy(saveFilePath, getAutosaveFile(config->indexDir));
    }
}

bool isLoadingFromSave(const char saveFilePath[MAX_FS_PATH_LENGTH]) {
    return FileExists(saveFilePath) && !config->forceNewGame;
}
