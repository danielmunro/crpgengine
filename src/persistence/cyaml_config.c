#include "headers/persistence/cyaml.h"

static const cyaml_config_t cyamlConfig = {
        .log_fn = cyaml_log,
        .mem_fn = cyaml_mem,
        .log_level = CYAML_LOG_WARNING,
};
