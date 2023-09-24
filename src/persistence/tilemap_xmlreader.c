typedef struct {
    xmlTextReaderPtr reader;
    Exploration *exploration;
} TilemapXmlReader;

TilemapXmlReader *createTilemapXmlReader(Exploration *exploration, const char *sceneFile) {
    addDebug("attempting to load scene file '%s'", sceneFile);
    TilemapXmlReader *reader = malloc(sizeof(TilemapXmlReader));
    reader->exploration = exploration;
    reader->reader = xmlReaderForFile(sceneFile, NULL, 0);
    return reader;
}
