#include <raylib.h>
#include "headers/attributes.h"
#include "headers/spell.h"

typedef struct {
    const char *id;
    const char *name;
    Texture2D image;
    Attributes *attributes;
    int level;
    int hp;
    int mana;
    int gp;
    int actionGauge;
    Rectangle position;
    ActionStep step;
    float hitAnimationTimer;
} Beast;

typedef struct {
    Beast *beasts[MAX_BEASTIARY_SIZE];
    int count;
} Beastiary;

Beast *createBeastFromData(BeastData *data) {
    Beast *beast = malloc(sizeof(Beast));
    beast->id = data->id;
    beast->name = data->name;
    beast->gp = data->gp;
    beast->hp = data->hp;
    beast->mana = data->mana;
    beast->level = data->level;
    beast->actionGauge = 0;
    char filePath[MAX_FS_PATH_LENGTH];
    sprintf(filePath, "%s/images/%s", config->indexDir, data->image);
    beast->image = LoadTextureFromImage(LoadImage(filePath));
    beast->attributes = createAttributesFromData(data->attributes);
    beast->position = (Rectangle) {0, 0, 0, 0};
    beast->step = STEP_NONE;
    beast->hitAnimationTimer = 0;
    return beast;
}

Beast *cloneBeast(const Beast *original) {
    Beast *new = malloc(sizeof(Beast));
    new->id = original->id;
    new->name = original->name;
    new->image = original->image;
    new->attributes = cloneAttributes(original->attributes);
    new->level = original->level;
    new->hp = original->hp;
    new->mana = original->mana;
    new->gp = original->gp;
    new->actionGauge = original->actionGauge;
    new->position = original->position;
    new->step = STEP_NONE;
    new->hitAnimationTimer = 0;
    return new;
}

Attributes calculateBeastAttributes(const Beast *beast) {
    return *beast->attributes;
}

void normalizeVitalsForBeast(Beast *beast) {
    Attributes calculated = calculateBeastAttributes(beast);
    if (beast->hp > calculated.hp) {
        beast->hp = calculated.hp;
    }
    if (beast->mana > calculated.mana) {
        beast->mana = calculated.mana;
    }
}

void executeSpellOnBeast(Beast *beast, const Spell *spell) {
    beast->hp += getSpellAttributeAmount(spell, spell->impact->hp);
    beast->mana += getSpellAttributeAmount(spell, spell->impact->mana);
    normalizeVitalsForBeast(beast);
}
