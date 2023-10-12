#include "headers/persistence/cyaml_config.h"
#include "headers/config.h"

MusicData *loadMusicYaml(const char *indexDir) {
    MusicData *music = malloc(sizeof(MusicData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/audio/music.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &musicTopSchema, (cyaml_data_t **) &music, NULL);
    if (err != CYAML_OK) {
        addError("error parsing music yaml :: %s, %s",
                filePath,
                cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return music;
}

SoundData *loadSoundYaml(const char *indexDir) {
    SoundData *sound = malloc(sizeof(SoundData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/audio/sound.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &musicTopSchema, (cyaml_data_t **) &sound, NULL);
    if (err != CYAML_OK) {
        addError("error parsing music yaml :: %s, %s",
                filePath,
                cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return sound;
}

AnimationData *loadAnimationYaml(const char *filePath) {
    AnimationData *animation = malloc(sizeof(AnimationData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &animationTopSchema, (cyaml_data_t **) &animation, NULL);
    if (err != CYAML_OK) {
        addError("error parsing animation yaml :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return animation;
}

SceneData *loadSceneYaml(const char *indexDir) {
    SceneData *scene = malloc(sizeof(SceneData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/scene.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &sceneTopSchema, (cyaml_data_t **) &scene, NULL);
    if (err != CYAML_OK) {
        addError("error parsing scene yaml :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return scene;
}

SpritesheetData *loadSpritesheetYaml(const char *filePath) {
    SpritesheetData *scene = malloc(sizeof(SpritesheetData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &spritesheetTopSchema, (cyaml_data_t **) &scene, NULL);
    if (err != CYAML_OK) {
        addError("error parsing spritesheet yaml :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return scene;
}

MobileData *loadMobYaml(const char *filePath) {
    MobileData *mob = malloc(sizeof(MobileData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &mobileTopSchema, (cyaml_data_t **) &mob, NULL);
    if (err != CYAML_OK) {
        addError("error parsing mob yaml from file :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return mob;
}

ItemsData *loadItemYaml(const char *filePath) {
    ItemsData *items = malloc(sizeof(ItemsData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &itemsSchema, (cyaml_data_t **) &items, NULL);
    if (err != CYAML_OK) {
        addError("error parsing item yaml :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return items;
}

BeastiaryData *loadBeastiaryYaml(const char *filePath) {
    BeastiaryData *beastiary = malloc(sizeof(BeastiaryData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &beastiaryTopSchema, (cyaml_data_t **) &beastiary, NULL);
    if (err != CYAML_OK) {
        addError("error parsing beast yaml :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return beastiary;
}

StorylinesData *loadStorylinesYaml(const char *filePath) {
    StorylinesData *storylines = malloc(sizeof(StorylinesData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &storylinesTopSchema, (cyaml_data_t **) &storylines, NULL);
    if (err != CYAML_OK) {
        addError("error loading file, filename and error :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlLoadStorylines);
    }
    return storylines;
}

SaveData *loadSaveData(const char *filePath) {
    SaveData *save = malloc(sizeof(SaveData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &saveTopSchema, (cyaml_data_t **) &save, NULL);
    if (err != CYAML_OK) {
        addError("error loading save data :: %s, %s",
                filePath,
                cyaml_strerror(err));
        exit(CyamlLoadPlayer);
    }
    if (save->player->storylines == NULL) {
        save->player->storylines = calloc(MAX_STORIES, sizeof(char *));
    }
    return save;
}

void saveSaveData(SaveData *saveData, const char *filePath) {
    addDebug("saving to %s", filePath);
    cyaml_err_t err = cyaml_save_file(filePath, &cyamlConfig,
                                      &saveTopSchema, saveData, 0);
    if (err != CYAML_OK) {
        addError("error saving player game data :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlSavePlayer);
    }
}

SpellsData *loadSpellData(const char *filePath) {
    SpellsData *spells = malloc(sizeof(SpellsData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &spellsTopSchema, (cyaml_data_t **) &spells, NULL);
    if (err != CYAML_OK) {
        addError("error loading file, filename and error :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return spells;
}

UIData *loadUIData() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/ui.yaml", config->indexDir);
    UIData *ui = malloc(sizeof(UIData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &uiSchema, (cyaml_data_t **) &ui, NULL);
    if (err != CYAML_OK) {
        addError("error loading file, filename and error :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return ui;
}

StartPartyData *loadStartPartyData() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/player/start_party.yaml", config->indexDir);
    StartPartyData *startParty = malloc(sizeof(StartPartyData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &startPartyTopSchema, (cyaml_data_t **) &startParty, NULL);
    if (err != CYAML_OK) {
        addError("error loading file, filename and error :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return startParty;
}

ItemsReferenceData *loadItemsReferenceData(const char *filePath) {
    ItemsReferenceData *items = malloc(sizeof(ItemsReferenceData));
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &itemsReferenceSchema, (cyaml_data_t **) &items, NULL);
    if (err != CYAML_OK) {
        addError("error parsing item yaml :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return items;
}
