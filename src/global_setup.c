void globalSetup(int argc, char *argv[]) {
    setupApp();
    createRuntimeArgs(argc, argv);
    createLog(runtimeArgs->logLevel);
    UIData *uiCfg = loadUIData();
    createUIConfig(uiCfg);
    initWindow(uiCfg->screen->title, uiCfg);
}
