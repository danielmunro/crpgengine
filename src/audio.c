#include <mm_malloc.h>
#include <raylib.h>
#include "headers/log.h"
#include "util/runtime_args.h"
#include "src/headers/cyaml.h"

typedef struct {
    Sound sound;
    char *name;
} Sound_;

typedef struct {
    Music music;
    char *name;
} Music_;

typedef struct {
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

void updateMusicStream(AudioManager *am) {
    if (am->musicIndex < 0) {
        addError("cannot play music with negative index :: %d", am->musicIndex);
        exit(GameEngineErrorMusicNotStreaming);
    }
    UpdateMusicStream(am->music[am->musicIndex]->music);
}

void playMusic(AudioManager *am, const char *name) {
    for (int i = 0; i < am->musicCount; i++) {
        if (strcmp(am->music[i]->name, name) == 0) {
            if (am->musicIndex == i) {
                return;
            }
            if (am->musicIndex > -1 && IsMusicStreamPlaying(am->music[am->musicIndex]->music)) {
                StopMusicStream(am->music[am->musicIndex]->music);
                addDebug("stopping music '%s'", am->music[am->musicIndex]->name);
            }
            am->musicIndex = i;
            PlayMusicStream(am->music[i]->music);
            addDebug("playing music '%s'", name);
            return;
        }
    }
    fprintf(stderr, "music not found\n");
}

void playSound(AudioManager *s, const char *name) {
    for (int i = 0; i < s->soundCount; i++) {
        if (strcmp(s->sounds[i]->name, name) == 0) {
            PlaySoundMulti(s->sounds[i]->sound);
        }
    }
}

void assignMusicValues(AudioManager *am) {
    MusicData *music = loadMusicYaml(runtimeArgs->indexDir);
    char filePath[MAX_FS_PATH_LENGTH] = "";
    sprintf(filePath, "%s/audio/%s", runtimeArgs->indexDir, music->town);

    Music_ *m = malloc(sizeof(Music_));
    m->name = "town";
    m->music = LoadMusicStream(filePath);

    am->music[am->musicCount] = m;
    am->musicCount++;
    free(music);
}

void assignSoundValues(AudioManager *am) {
    SoundData *sound = loadSoundYaml(runtimeArgs->indexDir);

//    Sound_ *s = malloc(sizeof(Sound_));
//    s->name = "bump";
//    s->sound = LoadMusicStream(sound->foo);
//    am->sounds[am->soundCount] = s;
//    am->soundCount++;
    free(sound);
}

void assignAudioManagerValues(AudioManager *am) {
    assignMusicValues(am);
    assignSoundValues(am);
}
