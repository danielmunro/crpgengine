void drawFightBackground(Encounters *encounters) {
    float scale = (float) ui->screen->width / (float) encounters->background.width;
    DrawTextureEx(encounters->background, (Vector2) {0, 0}, 0, scale, WHITE);
}

void drawFightBeasts(Fight *fight) {
    for (int i = 0; i < fight->beastCount; i++) {
        DrawTextureEx(fight->beasts[i]->image,
                      (Vector2) {
                              fight->beasts[i]->position.x,
                              fight->beasts[i]->position.y},
                      0, ui->screen->scale, WHITE);
    }
}

void drawFightPlayer(Player *player) {
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (player->party[i] != NULL) {
            drawAnimation(
                    findAnimation(getPartyLeader(player)->animations, LEFT),
                    getFightPlayerPosition(i, player->party[i]->animations[0]->spriteSheet->frameHeight));
        }
    }
}

void drawFightMenu(FightManager *fm) {
    drawAllMenus(fm->ui->menuContext, fm->menus);
}

void drawFightView(Encounters *encounters, FightManager *fights) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawFightBackground(encounters);
    drawFightBeasts(fights->fight);
    drawFightPlayer(fights->fight->player);
    drawFightMenu(fights);
    if (runtimeArgs->showFPS) {
        DrawFPS(FPS_X, FPS_Y);
    }
    EndDrawing();
}
