#ifndef CJRPGENGINE_ERRORS_H
#define CJRPGENGINE_ERRORS_H

typedef enum {
    // Argument errors
    ArgumentErrorIndexDirectoryRequired = 100,
    ArgumentErrorForceNewAndLoadSave = 101,

    // Runtime errors
    RuntimeErrorMobileNotFound = 200,
    RuntimeErrorNullSpellType = 201,
    RuntimeErrorNoSpellForType = 202,
    RuntimeErrorUnknownIntent = 203,
    RuntimeErrorUnknownItem = 205,
    RuntimeErrorUnknownEntrance = 206,

    // Game engine errors
    GameEngineErrorMenuNotDefined = 300,
    GameEngineErrorTextBoxNotFound = 301,
    GameEngineErrorMusicNotStreaming = 302,
    GameEngineErrorCannotAddMoreActiveControls = 303,

    // Configuration errors
    ConfigurationErrorUndefinedTextArea = 400,
    ConfigurationErrorMapResourcesMissing = 401,
    ConfigurationErrorMapResourcesUnreadable = 402,
    ConfigurationErrorLayerTypeNotFound = 403,
    ConfigurationErrorUnknownBeast = 404,
    ConfigurationErrorPlayerMobilesDirectoryDoesNotExist = 405,
    ConfigurationErrorUnknownSpriteSheet = 406,
    ConfigurationErrorMenuStyleNotDefined = 407,
    ConfigurationErrorUnknownWarp = 408,
    ConfigurationErrorUnknownScene = 409,
    ConfigurationErrorMissingComponent = 410,
    ConfigurationErrorUnknownFontStyleType = 411,
    ConfigurationErrorNoDirectionForName = 412,

    // Yaml loading/saving
    CyamlErrorGeneric = 500,
    CyamlSavePlayer = 501,
    CyamlLoadPlayer = 502,
    CyamlLoadStorylines = 503,
} ExitCode;

#endif //CJRPGENGINE_ERRORS_H
