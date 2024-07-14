#include <raylib.h>
#include "headers/test.h"
#include "headers/persistence/loader.h"
#include "headers/attributes.h"

AnimationManager *createTestAnimationManager(Context *c) {
    AnimationManager *am = createAnimationManager(c);
    SpritesheetManager *sm = loadSpritesheetManager(c);
    loadAllAnimations(am, sm);
    free(sm);
    return am;
}

Attributes *createTestAttributes() {
    Attributes *a = createEmptyAttributes();
    a->hp = STARTING_HP;
    a->mana = STARTING_MANA;
    a->strength = 5;
    return a;
}

bool doesBufferMatchSnapshot(bool updateSnapshots, char refFilename[MAX_FS_PATH_LENGTH]) {
    char refFilePath[MAX_FS_PATH_LENGTH];
    sprintf(refFilePath, "snapshots/%s", refFilename);

    SwapScreenBuffer();
    Image buf = LoadImageFromScreen();

    if (!FileExists(refFilePath)) {
        addInfo("snapshot does not exist, creating :: %s", refFilePath);
        ExportImage(buf, refFilePath);
        UnloadImage(buf);
        return true;
    }

    if (updateSnapshots) {
        addInfo("updating snapshot :: %s", refFilePath);
        ExportImage(buf, refFilePath);
        UnloadImage(buf);
        return true;
    }

    Image ref = LoadImage(refFilePath);

    if (buf.width != ref.width || buf.height != ref.height) {
        UnloadImage(buf);
        UnloadImage(ref);
        return false;
    }

    for (int y = 0; y < buf.height; y++) {
        for (int x = 0; x < buf.width; x++) {
            Color b = GetImageColor(buf, x, y);
            Color r = GetImageColor(ref, x, y);
            if (r.a != b.a || r.b != b.b || r.g != b.g || r.r != b.r) {
                UnloadImage(buf);
                UnloadImage(ref);
                return false;
            }
        }
    }

    UnloadImage(buf);
    UnloadImage(ref);
    return true;
}
