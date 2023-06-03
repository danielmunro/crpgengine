static const cyaml_config_t config = {
        .log_fn = cyaml_log,
        .mem_fn = cyaml_mem,
        .log_level = CYAML_LOG_WARNING,
};

Config *loadAppConfig(char *indexDir) {
    Config *appConfig = malloc(sizeof(Config));
    char *filepath = pathCat(indexDir, "/config.yaml");
    cyaml_err_t err = cyaml_load_file(filepath, &config,
                                      &appConfigTopSchema, (cyaml_data_t **)&appConfig, NULL);
    if (err != CYAML_OK) {
        fprintf(stderr, "error parsing app config file");
    }
    return appConfig;
}