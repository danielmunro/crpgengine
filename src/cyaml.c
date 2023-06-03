static const cyaml_config_t config = {
        .log_fn = cyaml_log,
        .mem_fn = cyaml_mem,
        .log_level = CYAML_LOG_WARNING,
};

ConfigYaml *loadAppConfig(char *indexDir) {
    ConfigYaml *appConfig = malloc(sizeof(ConfigYaml));
    char *filepath = pathCat(indexDir, "/config.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &appConfigTopSchema, (cyaml_data_t **)&appConfig, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing app config file");
    }
    return appConfig;
}

MusicYaml *loadMusicYaml(char *indexDir) {
    MusicYaml *music = malloc(sizeof(MusicYaml));
    char *filepath = pathCat(indexDir, "/music.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &musicTopSchema, (cyaml_data_t **)&music, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music config file: %s\n", cyaml_strerror(err));
    }
    return music;
}

SoundYaml *loadSoundYaml(char *indexDir) {
    SoundYaml *sound = malloc(sizeof(SoundYaml));
    char *filepath = pathCat(indexDir, "/sound.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &musicTopSchema, (cyaml_data_t **)&sound, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing music config file: %s\n", cyaml_strerror(err));
    }
    return sound;
}
