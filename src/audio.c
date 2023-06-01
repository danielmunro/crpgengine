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
} AudioManager;

AudioManager *createAudioManager() {
    AudioManager *s = malloc(sizeof(AudioManager));
    s->soundCount = 0;
    s->musicCount = 0;
    s->musicIndex = -1;
    return s;
}

void playMusic(AudioManager *am, char *name) {
    printf("play music %s requested, %d total songs\n", name, am->musicCount);
    for (int i = 0; i < am->musicCount; i++) {
        printf("music: %s\n", am->music[i]->name);
        if (strcmp(am->music[i]->name, name) == 0) {
            printf("music found, playing now\n");
            if (am->musicIndex > -1 && IsMusicStreamPlaying(am->music[am->musicIndex]->music)) {
                StopMusicStream(am->music[am->musicIndex]->music);
            }
            am->musicIndex = i;
            PlayMusicStream(am->music[i]->music);
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