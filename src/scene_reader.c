typedef struct {
    xmlTextReaderPtr reader;
    Exploration *exploration;
    int objectCount;
} SceneReader;

SceneReader *createSceneReader(Exploration *exploration, const char *sceneFile) {
    addDebug(exploration->log, "attempting to load scene file '%s'", sceneFile);
    SceneReader *sceneReader = malloc(sizeof(SceneReader));
    sceneReader->exploration = exploration;
    sceneReader->reader = xmlReaderForFile(sceneFile, NULL, 0);
    sceneReader->objectCount = 0;
    return sceneReader;
}
