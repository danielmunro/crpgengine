typedef struct Config {
    char *title;
} Config;

Config *createConfig() {
    Config *cfg = malloc(sizeof(Config));
    return cfg;
}