#include <tap.h>
#include "headers/global_setup.h"

void testMainMenuRenders(Context *c, bool updateSnapshots) {
    // setup
    Game *g = createGame(c);
    initializePlayer(g, NULL);
    initializeGameForPlayer(g);
    MenuContext *mc = g->ui->menuContext;

    // given
    findOrCreateTextBox(mc, MAIN_MENU_BOX, mc->context->ui->textAreas->full);

    // when
    BeginDrawing();
    drawMainMenuScreen(mc);
    EndDrawing();

    // then
    ok(doesBufferMatchSnapshot(
            updateSnapshots,
            "main_menu.png"));
}
