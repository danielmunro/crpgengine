typedef struct Sound_ {
    Sound sound;
    char *name;
} Sound_;

typedef struct Music_ {
    Music music;
    char *name;
} Music_;

typedef struct AudioManager {
    Sound_ *sounds[MAX_AUDIO];
    Music_ *music[MAX_AUDIO];
    int soundCount;
    int musicCount;
    int musicIndex;
    Log *log;
} AudioManager;

AudioManager *createAudioManager(Log *log) {
    AudioManager *s = malloc(sizeof(AudioManager));
    s->soundCount = 0;
    s->musicCount = 0;
    s->musicIndex = -1;
    s->log = log;
    return s;
}

void playMusic(AudioManager *am, char *name) {
    for (int i = 0; i < am->musicCount; i++) {
        if (strcmp(am->music[i]->name, name) == 0) {
            if (am->musicIndex == i) {
                return;
            }
            if (am->musicIndex > -1 && IsMusicStreamPlaying(am->music[am->musicIndex]->music)) {
                StopMusicStream(am->music[am->musicIndex]->music);
                addDebug(am->log, "stopping music '%s'", am->music[am->musicIndex]->name);
            }
            am->musicIndex = i;
            PlayMusicStream(am->music[i]->music);
            addDebug(am->log, "playing music '%s'", name);
            return;
        }
    }
    fprintf(stderr, "music not found\n");
}

void playSound(AudioManager *s, char *name) {
    for (int i = 0; i < s->soundCount; i++) {
        if (strcmp(s->sounds[i]->name, name) == 0) {
            PlaySoundMulti(s->sounds[i]->sound);
        }
    }
}

void assignMusicValues(AudioManager *am, const char *indexDir) {
    MusicData *music = loadMusicYaml(indexDir);
    char *filePath;
    sprintf(filePath, "%s/audio/%s", indexDir, music->town);

    Music_ *m = malloc(sizeof(Music_));
    m->name = "town";
    m->music = LoadMusicStream(filePath);

    am->music[am->musicCount] = m;
    am->musicCount++;
}

void assignSoundValues(AudioManager *am, char *indexDir) {
    SoundData *sound = loadSoundYaml(indexDir);

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
