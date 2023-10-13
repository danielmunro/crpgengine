#include <mm_malloc.h>
#include "headers/globals.h"
#include "headers/errors.h"
#include "headers/persistence/yaml/config.h"
#include "headers/util/log.h"

ConfigData *loadConfigYaml() {
    ConfigData *cfg = malloc(sizeof(ConfigData));
    char filePath[MAX_FS_PATH_LENGTH];
    // @todo check that path exists before attempting to load
    cyaml_err_t err = cyaml_load_file("config.yaml", &cyamlConfig,
                                      &configTopSchema, (cyaml_data_t **) &cfg, NULL);
    if (err != CYAML_OK) {
        addError("error parsing config yaml :: %s, %s",
                 filePath,
                 cyaml_strerror(err));
        exit(CyamlErrorGeneric);
    }
    return cfg;
}
