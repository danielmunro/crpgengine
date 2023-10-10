#ifndef CJRPGENGINE_INTENT_H
#define CJRPGENGINE_INTENT_H

#include <string.h>
#include "src/headers/log.h"
#include "src/headers/errors.h"

const char *Intents[] = {
        "harm",
        "help",
};

typedef enum {
    INTENT_HARM,
    INTENT_HELP,
} Intent;

Intent getIntentFromString(const char *intent) {
    if (strcmp(intent, "harm") == 0) {
        return INTENT_HARM;
    } else if (strcmp(intent, "help") == 0) {
        return INTENT_HELP;
    }
    addError("intent could not be found");
    exit(RuntimeErrorUnknownIntent);
}

#endif // CJRPGENGINE_INTENT_H
