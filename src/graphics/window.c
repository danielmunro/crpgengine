#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "headers/user_config.h"

void initWindow(
        Resolution resolution,
        bool fullScreen,
        const char *title,
        int targetFPS) {
    SetTargetFPS(targetFPS);
    InitWindow(
            resolution.width,
            resolution.height,
            title);
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("not ready");
        exit(1);
    }
    HideCursor();
    SetExitKey(0);
    if (fullScreen) {
        ToggleFullscreen();
    }
}
