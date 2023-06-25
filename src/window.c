void initWindow(const char *indexDir) {
    ConfigData *cfg = loadAppConfigYaml(indexDir);
    SetTargetFPS(TARGET_FRAMERATE);
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, cfg->title);
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("not ready");
        exit(1);
    }
    HideCursor();
    SetExitKey(0);
}

void postInitWindow() {
//    ToggleFullscreen();
}