typedef struct {
    xmlTextReaderPtr reader;
    Exploration *exploration;
    int objectCount;
} TilemapXmlReader;

TilemapXmlReader *createTilemapXmlReader(Exploration *exploration, const char *sceneFile) {
    addDebug(exploration->log, "attempting to load scene file '%s'", sceneFile);
    TilemapXmlReader *reader = malloc(sizeof(TilemapXmlReader));
    reader->exploration = exploration;
    reader->reader = xmlReaderForFile(sceneFile, NULL, 0);
    reader->objectCount = 0;
    return reader;
}
