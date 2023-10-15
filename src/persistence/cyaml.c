#include "headers/persistence/cyaml_config.h"
#include "headers/config.h"
#include "headers/errors.h"

void loadYamlFile(const char *filePath, cyaml_schema_value_t schema, cyaml_data_t **dataOut) {
    cyaml_err_t err = cyaml_load_file(filePath, &cyamlConfig,
                                      &schema, dataOut, NULL);
    if (err != CYAML_OK) {
        addError("error parsing yaml :: %s, %s",
                 filePath,
                 cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
}

MusicData *loadMusicYaml(const char *indexDir) {
    MusicData *music = malloc(sizeof(MusicData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/audio/music.yaml", indexDir);
    loadYamlFile(filePath, musicTopSchema, (cyaml_data_t **) &music);
    return music;
}

SoundData *loadSoundYaml(const char *indexDir) {
    SoundData *sound = malloc(sizeof(SoundData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/audio/sound.yaml", indexDir);
    loadYamlFile(filePath, soundTopSchema, (cyaml_data_t **) &sound);
    return sound;
}

AnimationData *loadAnimationYaml(const char *filePath) {
    AnimationData *animation = malloc(sizeof(AnimationData));
    loadYamlFile(filePath, animationTopSchema, (cyaml_data_t **) &animation);
    return animation;
}

SceneData *loadSceneYaml(const char *indexDir) {
    SceneData *scene = malloc(sizeof(SceneData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/scene.yaml", indexDir);
    loadYamlFile(filePath, sceneTopSchema, (cyaml_data_t **) &scene);
    return scene;
}

SpritesheetData *loadSpritesheetYaml(const char *filePath) {
    SpritesheetData *scene = malloc(sizeof(SpritesheetData));
    loadYamlFile(filePath, spritesheetTopSchema, (cyaml_data_t **) &scene);
    return scene;
}

MobileData *loadMobYaml(const char *filePath) {
    MobileData *mob = malloc(sizeof(MobileData));
    loadYamlFile(filePath, mobileTopSchema, (cyaml_data_t **) &mob);
    return mob;
}

ItemsData *loadItemYaml() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/items.yaml", config->indexDir);
    ItemsData *items = malloc(sizeof(ItemsData));
    loadYamlFile(filePath, itemsSchema, (cyaml_data_t **) &items);
    return items;
}

BeastiaryData *loadBeastiaryYaml() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/beastiary.yaml", config->indexDir);
    BeastiaryData *beastiary = malloc(sizeof(BeastiaryData));
    loadYamlFile(filePath, beastiaryTopSchema, (cyaml_data_t **) &beastiary);
    return beastiary;
}

StorylinesData *loadStorylinesYaml(const char *filePath) {
    StorylinesData *storylines = malloc(sizeof(StorylinesData));
    loadYamlFile(filePath, storylinesTopSchema, (cyaml_data_t **) &storylines);
    return storylines;
}

SaveData *loadSaveData(const char *filePath) {
    SaveData *save = malloc(sizeof(SaveData));
    loadYamlFile(filePath, saveTopSchema, (cyaml_data_t **) &save);
    if (save->player->storylines == NULL) {
        save->player->storylines = calloc(MAX_STORIES, sizeof(char *));
    }
    return save;
}

void saveSaveData(const SaveData *saveData, const char *filePath) {
    addDebug("saving to %s", filePath);
    cyaml_err_t err = cyaml_save_file(filePath, &cyamlConfig,
                                      &saveTopSchema, saveData, 0);
    if (err != CYAML_OK) {
        addError("error saving player game data :: %s, %s",
                filePath, cyaml_strerror(err));
        exit(CyamlSavePlayer);
    }
}

SpellsData *loadSpellData() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/spells.yaml", config->indexDir);
    SpellsData *spells = malloc(sizeof(SpellsData));
    loadYamlFile(filePath, spellsTopSchema, (cyaml_data_t **) &spells);
    return spells;
}

UIData *loadUIData() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/ui.yaml", config->indexDir);
    UIData *ui = malloc(sizeof(UIData));
    loadYamlFile(filePath, uiSchema, (cyaml_data_t **) &ui);
    return ui;
}

StartPartyData *loadStartPartyData() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/player/start_party.yaml", config->indexDir);
    StartPartyData *startParty = malloc(sizeof(StartPartyData));
    loadYamlFile(filePath, startPartyTopSchema, (cyaml_data_t **) &startParty);
    return startParty;
}

ItemsReferenceData *loadItemsReferenceData() {
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/player/items.yaml", config->indexDir);
    ItemsReferenceData *items = malloc(sizeof(ItemsReferenceData));
    loadYamlFile(filePath, itemsReferenceSchema, (cyaml_data_t **) &items);
    return items;
}
