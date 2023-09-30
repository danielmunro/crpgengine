#ifndef CJRPGENGINE_ERRORS_H
#define CJRPGENGINE_ERRORS_H

typedef enum {
    // Argument errors
    ArgumentErrorIndexDirectoryRequired = 100,
    ArgumentErrorForceNewAndLoadSave = 101,

    // Runtime errors
    RuntimeErrorMenuStyleNotDefined = 200,
    RuntimeErrorMobileNotFound = 201,
    RuntimeErrorNullSpellType = 202,
    RuntimeErrorNoSpellForType = 203,
    RuntimeErrorUnknownIntent = 204,
    RuntimeErrorUnknownCollisionType = 205,
    RuntimeErrorUnknownItem = 206,
    RuntimeErrorUnknownEntrance = 207,
    RuntimeErrorUnknownScene = 208,
    RuntimeErrorUnknownWarp = 209,

    // Game engine errors
    GameEngineErrorMenuNotDefined = 300,
    GameEngineErrorTextBoxNotFound = 301,
    GameEngineErrorMusicNotStreaming = 302,
    GameEngineErrorCannotAddMoreActiveControls = 303,

    // Configuration errors
    ConfigurationErrorUndefinedTextArea = 400,
    ConfigurationErrorMapResourcesMissing = 401,
    ConfigurationErrorMapResourcesUnreadable = 402,
    ConfigurationErrorUnknownLayer = 403,
    ConfigurationErrorUnknownBeast = 404,
    ConfigurationErrorPlayerMobilesDirectoryDoesNotExist = 405,
    ConfigurationErrorUnknownSpriteSheet = 406,

    // Yaml loading/saving
    CyamlErrorGeneric = 500,
    CyamlSavePlayer = 501,
    CyamlLoadPlayer = 502,
    CyamlLoadStorylines = 503,
} ExitCode;

#endif //CJRPGENGINE_ERRORS_H
