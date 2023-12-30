#include "headers/graphics/ui/ui.h"
#include "headers/config.h"

void globalSetup(const UserConfig *userConfig) {
    UIData *uiCfg = loadUIData();
    createUIConfig(uiCfg, userConfig);
    initWindow(uiCfg->screen->title, uiCfg, userConfig);
    if (userConfig->fullScreen) {
        ToggleFullscreen();
    }
}
