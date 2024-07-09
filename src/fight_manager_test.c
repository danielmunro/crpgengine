void testCanFleeSuccessfully(Context *c) {
    // setup
    Game *g = createGame(c);
    initializePlayer(g, NULL);
    initializeGameForPlayer(g);

    // given
    Beast **beasts = calloc(1, sizeof(Beast));
    beasts[0] = createTestBeast();
    Fight *f = createFight(c, beasts, g->player, 1);
    g->fights->fight = f;
    ActionParticipant *playerParticipant = createMobParticipant(f->player->party[0]);
    ActionParticipant *beastParticipant = createBeastParticipant(beasts[0], 0);
    ok(g->notifications->count == 0);
    int attempt = 100;

    // when
    while (attempt > 0 && !tryFlee(g->fights, createAction(RUN, ACTION_STEP_RUN,  playerParticipant, beastParticipant, NULL))) {
        attempt--;
    }

    // then
    ok(g->notifications->count > 0);

    // and
    ok(g->fights->fight->cancelled);
}

void testCanFailAttemptingToFlee(Context *c) {
    // setup
    Game *g = createGame(c);
    initializePlayer(g, NULL);
    initializeGameForPlayer(g);

    // given
    int attempt = 100;
    bool fighting = true;

    // when
    while (attempt > 0 && fighting) {
        Beast **beasts = calloc(1, sizeof(Beast));
        beasts[0] = createTestBeast();
        Fight *f = createFight(c, beasts, g->player, 1);
        g->fights->fight = f;
        ActionParticipant *playerParticipant = createMobParticipant(f->player->party[0]);
        ActionParticipant *beastParticipant = createBeastParticipant(beasts[0], 0);
        fighting = tryFlee(
                g->fights,
                createAction(
                        RUN,
                        ACTION_STEP_RUN,
                        playerParticipant,
                        beastParticipant,
                        NULL
                    )
                );
        attempt--;
    }

    // then
    ok(g->notifications->count > 0);

    // and
    ok(!g->fights->fight->cancelled);
}
