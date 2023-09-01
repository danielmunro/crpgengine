#ifndef CJRPGENGINE_INTENT_H
#define CJRPGENGINE_INTENT_H

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
    fprintf(stderr, "intent could not be found");
    exit(EXIT_UNKNOWN_INTENT);
}

#endif // CJRPGENGINE_INTENT_H
