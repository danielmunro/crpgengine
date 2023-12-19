#include <string.h>

const char *Difficulties[] = {
        "casual",
        "normal",
        "challenge",
};

typedef enum {
    DIFFICULTY_CASUAL,
    DIFFICULTY_NORMAL,
    DIFFICULTY_CHALLENGE,
} Difficulty;

typedef struct {
    int width;
    int height;
} Resolution;

typedef struct {
    bool fullScreen;
    Resolution *resolution;
    Difficulty difficulty;
} UserConfig;

UserConfig *createUserConfigFromData(UserConfigData *data) {
    UserConfig *userConfig = malloc(sizeof(UserConfig));
    userConfig->fullScreen = data->fullScreen;
    userConfig->resolution = malloc(sizeof(Resolution));
    userConfig->resolution->width = data->resolution->width;
    userConfig->resolution->height = data->resolution->height;
    int difficultyCount = sizeof(Difficulties) / sizeof(Difficulties[0]);
    for (int i = 0; i < difficultyCount; i++) {
        if (strcmp(data->difficulty, Difficulties[i]) == 0) {
            userConfig->difficulty = i;
            break;
        }
    }
    return userConfig;
}
