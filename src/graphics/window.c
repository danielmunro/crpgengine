#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "headers/persistence/cyaml.h"
#include "headers/user_config.h"

void initWindow(const char *title, const UIData *ui, const UserConfig *userConfig) {
    SetTargetFPS(ui->screen->targetFrameRate);
    InitWindow(userConfig->resolution.width, userConfig->resolution.height, title);
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("not ready");
        exit(1);
    }
    HideCursor();
    SetExitKey(0);
}
