#include <stdbool.h>
#include "cyaml/cyaml.h"

typedef struct {
    const char *player;
    const char *mob;
    const char *condition;
    const char *story;
    const char *scene;
    ItemReferenceData *item;
    const char *arriveAt;
} WhenData;

typedef struct {
    bool player;
    bool parallel;
    const char *mob;
    const char *action;
    const char *dialog;
    const char *story;
    const char *position;
    const char *direction;
    ItemReferenceData *item;
    int amount;
} ThenData;

typedef struct {
    WhenData *when;
    ThenData *then;
    int when_count;
    int then_count;
} StorylineData;

typedef struct {
    StorylineData *storylines;
    int storylines_count;
} StorylinesData;

static const cyaml_schema_field_t whenFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "player", CYAML_FLAG_OPTIONAL, WhenData, player, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "condition", CYAML_FLAG_OPTIONAL, WhenData, condition, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "mob", CYAML_FLAG_OPTIONAL, WhenData, mob, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "story", CYAML_FLAG_OPTIONAL, WhenData, story, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "item", CYAML_FLAG_OPTIONAL, WhenData, item, itemReferenceFieldSchema),
        CYAML_FIELD_STRING_PTR(
                "scene", CYAML_FLAG_OPTIONAL, WhenData, scene, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "arrive_at", CYAML_FLAG_OPTIONAL, WhenData, arriveAt, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t whenSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, WhenData, whenFieldSchema),
};

static const cyaml_schema_field_t thenFieldSchema[] = {
        CYAML_FIELD_STRING_PTR(
                "mob", CYAML_FLAG_OPTIONAL, ThenData, mob, 0, CYAML_UNLIMITED),
        CYAML_FIELD_BOOL(
                "player", CYAML_FLAG_OPTIONAL, ThenData, player),
        CYAML_FIELD_BOOL(
                "parallel", CYAML_FLAG_OPTIONAL, ThenData, parallel),
        CYAML_FIELD_STRING_PTR(
                "action", CYAML_FLAG_OPTIONAL, ThenData, action, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "dialog", CYAML_FLAG_OPTIONAL, ThenData, dialog, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "story", CYAML_FLAG_OPTIONAL, ThenData, story, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "position", CYAML_FLAG_OPTIONAL, ThenData, position, 0, CYAML_UNLIMITED),
        CYAML_FIELD_STRING_PTR(
                "direction", CYAML_FLAG_OPTIONAL, ThenData, direction, 0, CYAML_UNLIMITED),
        CYAML_FIELD_MAPPING_PTR(
                "item", CYAML_FLAG_POINTER | CYAML_FLAG_OPTIONAL,
                ThenData, item, itemReferenceFieldSchema),
        CYAML_FIELD_INT(
                "amount", CYAML_FLAG_OPTIONAL, ThenData, amount),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t thenSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT, WhenData, thenFieldSchema),
};

static const cyaml_schema_field_t storylinesFieldSchema[] = {
        CYAML_FIELD_SEQUENCE(
                "when", CYAML_FLAG_POINTER, StorylineData, when, &whenSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_SEQUENCE(
                "then", CYAML_FLAG_POINTER, StorylineData, then, &thenSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t storylinesSchema = {
        CYAML_VALUE_MAPPING(CYAML_FLAG_DEFAULT,
                            WhenData, storylinesFieldSchema),
};

static const cyaml_schema_field_t storylinesTopMappingField[] = {
        CYAML_FIELD_SEQUENCE(
                "storylines", CYAML_FLAG_POINTER, StorylinesData, storylines, &storylinesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t storylinesTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, StorylinesData, storylinesTopMappingField),
};
