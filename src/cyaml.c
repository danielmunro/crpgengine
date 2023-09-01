static const cyaml_config_t config = {
        .log_fn = cyaml_log,
        .mem_fn = cyaml_mem,
        .log_level = CYAML_LOG_WARNING,
};

ConfigData *loadAppConfigYaml(const char *indexDir) {
    ConfigData *appConfig = malloc(sizeof(ConfigData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/config.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &appConfigTopSchema, (cyaml_data_t **) &appConfig, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing app config yaml\n");
    }
    return appConfig;
}

MusicData *loadMusicYaml(const char *indexDir) {
    MusicData *music = malloc(sizeof(MusicData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/audio/music.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &musicTopSchema, (cyaml_data_t **) &music, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music yaml: %s\n", cyaml_strerror(err));
    }
    return music;
}

SoundData *loadSoundYaml(const char *indexDir) {
    SoundData *sound = malloc(sizeof(SoundData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/audio/sound.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &musicTopSchema, (cyaml_data_t **) &sound, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music yaml: %s\n", cyaml_strerror(err));
    }
    return sound;
}

AnimationData *loadAnimationYaml(const char *filePath) {
    AnimationData *animation = malloc(sizeof(AnimationData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &animationTopSchema, (cyaml_data_t **) &animation, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing animation yaml at %s :: %s\n", filePath, cyaml_strerror(err));
    }
    return animation;
}

SceneData *loadSceneYaml(const char *indexDir) {
    SceneData *scene = malloc(sizeof(SceneData));
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/scene.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &sceneTopSchema, (cyaml_data_t **) &scene, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing scene yaml\n");
    }
    return scene;
}

SpritesheetData *loadSpritesheetYaml(const char *filePath) {
    SpritesheetData *scene = malloc(sizeof(SpritesheetData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &spritesheetTopSchema, (cyaml_data_t **) &scene, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing spritesheet yaml\n");
    }
    return scene;
}

MobileData *loadMobYaml(const char *filePath) {
    MobileData *mob = malloc(sizeof(MobileData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &mobileTopSchema, (cyaml_data_t **) &mob, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing mob yaml\n");
    }
    return mob;
}

ItemsData *loadItemYaml(const char *filePath) {
    ItemsData *items = malloc(sizeof(ItemsData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &itemsSchema, (cyaml_data_t **) &items, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing item yaml\n");
    }
    return items;
}

BeastiaryData *loadBeastiaryYaml(const char *filePath) {
    BeastiaryData *beastiary = malloc(sizeof(BeastiaryData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &beastiaryTopSchema, (cyaml_data_t **) &beastiary, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing beast yaml\n");
    }
    return beastiary;
}

StorylinesData *loadStorylinesYaml(const char *filePath) {
    StorylinesData *storylines = malloc(sizeof(StorylinesData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &storylinesTopSchema, (cyaml_data_t **) &storylines, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error loading file, filename and error :: %s - %s\n", filePath, cyaml_strerror(err));
        exit(1);
    }
    return storylines;
}

SaveData *loadSaveData(const char *filePath) {
    SaveData *save = malloc(sizeof(SaveData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &saveTopSchema, (cyaml_data_t **) &save, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "%s\n", cyaml_strerror(err));
        return NULL;
    }
    if (save == NULL) {
        fprintf(stderr, "error loading save file");
        return NULL;
    }
    if (save->player->storylines == NULL) {
        save->player->storylines = calloc(MAX_STORIES, sizeof(char *));
    }
    return save;
}

void saveSaveData(SaveData *saveData, const char *filePath) {
    cyaml_err_t err = cyaml_save_file(filePath, &config,
                    &saveTopSchema, saveData, 0);
    if (err != CYAML_OK) {
        fprintf(stderr, "%s\n", cyaml_strerror(err));
    }
}

SpellsData *loadSpellData(const char *filePath) {
    SpellsData *spells = malloc(sizeof(SpellsData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &spellsTopSchema, (cyaml_data_t **) &spells, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error loading file, filename and error :: %s - %s\n", filePath, cyaml_strerror(err));
        exit(1);
    }
    return spells;
}
