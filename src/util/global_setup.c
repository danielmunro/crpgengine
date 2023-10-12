#include "headers/persistence/load_config.h"
#include "headers/graphics/ui/ui.h"
#include "headers/config.h"

void globalSetup(int argc, char *argv[]) {
    SetTraceLogLevel(LOG_WARNING);
    createConfigFromData(loadConfigYaml());
    createLog(config->logLevel);
    UIData *uiCfg = loadUIData();
    createUIConfig(uiCfg);
    initWindow(uiCfg->screen->title, uiCfg);
}
