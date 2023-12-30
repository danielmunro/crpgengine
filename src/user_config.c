#include <string.h>
#include <mm_malloc.h>

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
    Resolution resolution;
    Difficulty difficulty;
} UserConfig;

UserConfig *createUserConfigFromData(UserConfigData *data) {
    UserConfig *userConfig = malloc(sizeof(UserConfig));
    userConfig->fullScreen = data->fullScreen;
    userConfig->resolution = (Resolution) {
        data->resolution->width,
        data->resolution->height,
    };
    int difficultyCount = sizeof(Difficulties) / sizeof(Difficulties[0]);
    for (int i = 0; i < difficultyCount; i++) {
        if (strcmp(data->difficulty, Difficulties[i]) == 0) {
            userConfig->difficulty = i;
            break;
        }
    }
    return userConfig;
}

UserConfigData *createUserConfigData(const UserConfig *cfg) {
    UserConfigData *data = malloc(sizeof(UserConfigData));
    data->fullScreen = cfg->fullScreen;
    data->resolution = malloc(sizeof(ResolutionData));
    data->resolution->height = cfg->resolution.height;
    data->resolution->width = cfg->resolution.width;
    data->difficulty = Difficulties[cfg->difficulty];
    return data;
}

void saveUserConfig(const UserConfig *cfg) {
    UserConfigData *data = createUserConfigData(cfg);
    saveUserConfigData(data);
    free(data);
}

UserConfig *createUserConfig() {
    UserConfigData *data = loadUserConfigYaml();
    UserConfig *userConfig = createUserConfigFromData(data);
    free(data);
    return userConfig;
}
