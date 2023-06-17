static const cyaml_config_t config = {
        .log_fn = cyaml_log,
        .mem_fn = cyaml_mem,
        .log_level = CYAML_LOG_WARNING,
};

ConfigData *loadAppConfigYaml(const char *indexDir) {
    ConfigData *appConfig = malloc(sizeof(ConfigData));
    char filePath[255];
    sprintf(filePath, "%s/config.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &appConfigTopSchema, (cyaml_data_t **)&appConfig, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing app config yaml\n");
    }
    return appConfig;
}

MusicData *loadMusicYaml(const char *indexDir) {
    MusicData *music = malloc(sizeof(MusicData));
    char filePath[255];
    sprintf(filePath, "%s/music.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &musicTopSchema, (cyaml_data_t **)&music, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music yaml: %s\n", cyaml_strerror(err));
    }
    return music;
}

SoundData *loadSoundYaml(const char *indexDir) {
    SoundData *sound = malloc(sizeof(SoundData));
    char filePath[255];
    sprintf(filePath, "%s/sound.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &musicTopSchema, (cyaml_data_t **)&sound, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music yaml: %s\n", cyaml_strerror(err));
    }
    return sound;
}

PlayerData *loadPlayerYaml(const char *indexDir) {
    PlayerData *player = malloc(sizeof(PlayerData));
    char filePath[255];
    sprintf(filePath, "%s/player.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &playerTopSchema, (cyaml_data_t **)&player, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing player config file: %s\n", cyaml_strerror(err));
    }
    return player;
}

AnimationData *loadAnimationYaml(const char *filePath) {
    AnimationData *animation = malloc(sizeof(AnimationData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &animationTopSchema, (cyaml_data_t **)&animation, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing animation yaml: %s\n", cyaml_strerror(err));
    }
    return animation;
}

SceneData *loadSceneYaml(const char *indexDir) {
    SceneData *scene = malloc(sizeof(SceneData));
    char filePath[255];
    sprintf(filePath, "%s/scene.yaml", indexDir);
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &sceneTopSchema, (cyaml_data_t **)&scene, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing scene yaml\n");
    }
    return scene;
}

MobileData *loadMobYaml(const char *filePath) {
    MobileData *mob = malloc(sizeof(MobileData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &mobileTopSchema, (cyaml_data_t **)&mob, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing mob yaml\n");
    }
    return mob;
}

BeastiaryData *loadBeastiaryYaml(const char *filePath) {
    BeastiaryData *beastiary = malloc(sizeof(BeastiaryData));
    cyaml_err_t err = cyaml_load_file(filePath, &config,
                                      &beastiaryTopSchema, (cyaml_data_t **)&beastiary, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing beast yaml\n");
    }
    return beastiary;
}
