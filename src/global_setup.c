UIData *globalSetup(int argc, char *argv[]) {
    setupApp();
    createRuntimeArgs(argc, argv);
    createLog(runtimeArgs->logLevel);
    ConfigData *appCfg = loadAppConfigYaml();
    UIData *uiCfg = loadUIData();
    initWindow(appCfg->title, uiCfg);
    return uiCfg;
}
