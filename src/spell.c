typedef struct {
    SpellType type;
    int level;
    int cost;
} MobileSpell;

typedef struct {
    SpellType type;
    const char *name;
    Intent intent;
    int level;
} Spell;
