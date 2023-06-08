void clean(char *value) {
    while (true) {
        if (value[0] != ' ') {
            break;
        }
        memmove(&value[0], &value[1], strlen(value));
    }
    if (value[0] == '"') {
        memmove(&value[0], &value[1], strlen(value));
        unsigned long l = strlen(value);
        if (value[l - 1] == '"') {
            memmove(&value[l - 1], &value[l], 1);
        }
    }
}

void parseKV(char *data, char *result[2]) {
    result[0] = strtok(data, ":");
    result[1] = strtok(NULL, "\r\n");
    clean(result[0]);
    if (result[1] != NULL) {
        clean(result[1]);
    }
}

int parseKVPairs(char *data, char *result[255]) {
    char *rows[255];
    char *row = strtok(data, "\r\n");
    int r = 0;
    while(row != NULL) {
        rows[r] = row;
        row = strtok(NULL, "\r\n");
        r++;
    }
    int j = 0;
    for (int i = 0; i < r; i++) {
        char *kv[2];
        parseKV(rows[i], kv);
        if (kv[0][0] != '#') {
            result[j] = kv[0];
            result[j + 1] = kv[1];
            printf("kv: %s, %s\n", kv[0], kv[1]);
            j += 2;
        }
    }
    return j;
}

void assignMusicValues(AudioManager *am, char *indexDir) {
    MusicYaml *music = loadMusicYaml(indexDir);
    char *basePath = pathCat(indexDir, "audio");
    char *filePath = pathCat(basePath, music->town);

    Music_ *m = malloc(sizeof(Music_));
    m->name = "town";
    m->music = LoadMusicStream(filePath);

    am->music[am->musicCount] = m;
    am->musicCount++;
}

void assignSoundValues(AudioManager *am, char *indexDir) {
    SoundYaml *sound = loadSoundYaml(indexDir);

//    Sound_ *s = malloc(sizeof(Sound_));
//    s->name = "bump";
//    s->sound = LoadMusicStream(sound->foo);
//    am->sounds[am->soundCount] = s;
//    am->soundCount++;
}

void assignAudioManagerValues(AudioManager *am, char *indexDir) {
    assignMusicValues(am, indexDir);
    assignSoundValues(am, indexDir);
}
