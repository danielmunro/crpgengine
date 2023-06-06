static const cyaml_config_t config = {
        .log_fn = cyaml_log,
        .mem_fn = cyaml_mem,
        .log_level = CYAML_LOG_WARNING,
};

ConfigYaml *loadAppConfig(const char *indexDir) {
    ConfigYaml *appConfig = malloc(sizeof(ConfigYaml));
    char *filepath = pathCat(indexDir, "/config.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &appConfigTopSchema, (cyaml_data_t **)&appConfig, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing app config file\n");
    }
    return appConfig;
}

MusicYaml *loadMusicYaml(const char *indexDir) {
    MusicYaml *music = malloc(sizeof(MusicYaml));
    char *filepath = pathCat(indexDir, "/music.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &musicTopSchema, (cyaml_data_t **)&music, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music config file: %s\n", cyaml_strerror(err));
    }
    return music;
}

SoundYaml *loadSoundYaml(const char *indexDir) {
    SoundYaml *sound = malloc(sizeof(SoundYaml));
    char *filepath = pathCat(indexDir, "/sound.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &musicTopSchema, (cyaml_data_t **)&sound, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music config file: %s\n", cyaml_strerror(err));
    }
    return sound;
}

PlayerYaml *loadPlayerYaml(const char *indexDir) {
    PlayerYaml *player = malloc(sizeof(PlayerYaml));
    char *filepath = pathCat(indexDir, "/player.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &playerTopSchema, (cyaml_data_t **)&player, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing player config file: %s\n", cyaml_strerror(err));
    }
    return player;
}

AnimationYaml *loadAnimationYaml(const char *filepath) {
    AnimationYaml *animation = malloc(sizeof(AnimationYaml));
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &animationTopSchema, (cyaml_data_t **)&animation, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing animation config file: %s\n", cyaml_strerror(err));
    }
    return animation;
}

SceneYaml *loadSceneYaml(const char *indexDir) {
    SceneYaml *scene = malloc(sizeof(SceneYaml));
    char *filepath = pathCat(indexDir, "/scene.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &sceneTopSchema, (cyaml_data_t **)&scene, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing scene config file\n");
    }
    return scene;
}

//MobileYaml *loadMobYaml(const char *filepath) {
//    MobileYaml *mob = malloc(sizeof(MobileYaml));
//    cyaml_err_t err = cyaml_load_file(filepath, &config,
//                                      &mobileTopSchema, (cyaml_data_t **)&mob, NULL);
//    if (err != CYAML_OK) {
//        fprintf(stderr, "error parsing mob config file\n");
//    }
//    return mob;
//}

WhenYaml *loadWhenYaml(const char *filepath) {
    WhenYaml *when = malloc(sizeof(WhenYaml));
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &mobileTopSchema, (cyaml_data_t **)&when, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing mob config file\n");
    }
    return when;
}
