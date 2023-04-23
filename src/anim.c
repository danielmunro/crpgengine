
Mobile loadAnimations(const char *file, Animation *animations[MAX_ANIMATIONS]) {
    char *data = LoadFileText(file);
    char *spriteSheetName = strtok(data, ",");
    char *width = strtok(NULL, ",");
    char *height = strtok(NULL, "\r\n");
    SpriteSheet *sp = createSpriteSheet(spriteSheetName, strToInt(width), strToInt(height));
    int anim = 0;
    while (true) {
        char *name = strtok(NULL, ",");
        if (name == NULL) {
            break;
        }
        char *firstFrame = strtok(NULL, ",");
        char *lastFrame = strtok(NULL, ",");
        char *frameRate = strtok(NULL, ",");
        char *repeat = strtok(NULL, "\r\n");
        animations[anim] = malloc(sizeof(Animation));
        animations[anim] = createAnimation(
                sp,
                getAnimIdFromName(name),
                atoi(firstFrame),
                atoi(lastFrame),
                atoi(frameRate),
                atoi(repeat)
        );
        anim++;
    }
    printf("%d animations loaded\n", anim);
}
