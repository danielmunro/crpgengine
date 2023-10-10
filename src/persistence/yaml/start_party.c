#include "cyaml/cyaml.h"

typedef struct {
    const char **mobiles;
    int mobiles_count;
} StartPartyData;

static const cyaml_schema_value_t mobilesSchema = {
        CYAML_VALUE_STRING(CYAML_FLAG_POINTER, char, 0, CYAML_UNLIMITED),
};

static const cyaml_schema_field_t startPartyTopMappingField[] = {
        CYAML_FIELD_SEQUENCE(
                "mobiles", CYAML_FLAG_POINTER,
                StartPartyData, mobiles, &mobilesSchema, 0, CYAML_UNLIMITED),
        CYAML_FIELD_END
};

static const cyaml_schema_value_t startPartyTopSchema = {
        CYAML_VALUE_MAPPING(
                CYAML_FLAG_POINTER, StartPartyData, startPartyTopMappingField),
};
