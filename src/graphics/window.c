#include <raylib.h>
#include <stdio.h>
#include <stdlib.h>
#include "src/headers/cyaml.h"

void initWindow(const char *title, const UIData *ui) {
    SetTargetFPS(ui->screen->targetFrameRate);
    InitWindow(ui->screen->width, ui->screen->height, title);
    InitAudioDevice();
    if (!IsAudioDeviceReady()) {
        printf("not ready");
        exit(1);
    }
    HideCursor();
    SetExitKey(0);
    if (ui->screen->full) {
        ToggleFullscreen();
    }
}
