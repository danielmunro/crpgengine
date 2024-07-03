#include <tap.h>
#include "headers/player.h"
#include "headers/player_manager.h"
#include "headers/global_setup.h"

void testCanSavePlayer(Context *c) {
    // setup
    Game *g = createGame(c);
    initializePlayer(g, NULL);
    initializeGameForPlayer(g);

    // given
    const Player *player = createNewPlayer(g->mobiles, g->items);

    // when
    addInfo("start save");
    const SaveFile *sf = save(player, g->scenes->current->name, c->indexDir);

    // then
    ok(sf != NULL);

    // and
    char *filepath = malloc(MAX_FS_PATH_LENGTH);
    sprintf(filepath, "%s/%s/%s", c->indexDir, "_saves", sf->filename);
    ok(access(filepath, F_OK) == 0);

    // and
    ok(sf->created == (unsigned long) time(NULL));

    // and
    ok(sizeof(sf->saveName) > 0);
}

void testCanLoadPlayer(Context *c) {
    // setup
    Game *g = createGame(c);

    // given
    initializePlayer(g, g->saveFiles->saves[0]->filename);

    // when
    initializeGameForPlayer(g);

    // then
    ok(g->scenes != NULL);
    ok(g->controls != NULL);
    ok(strcmp(g->scenes->current->name, g->saveToLoad->scene) == 0);
}
