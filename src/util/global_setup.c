void globalSetup(int argc, char *argv[]) {
    SetTraceLogLevel(LOG_WARNING);
    createRuntimeArgs(argc, argv);
    createLog(runtimeArgs->logLevel);
    UIData *uiCfg = loadUIData();
    createUIConfig(uiCfg);
    initWindow(uiCfg->screen->title, uiCfg);
}
