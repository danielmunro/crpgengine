void drawPlayer(Player *player) {
    drawAnimation(
            findAnimation(player->mob->animations, DOWN),
            (Vector2){20, 20}
            );
}

void drawStats(Player *player) {
    drawText(player->mob->name, (Vector2d){(UI_PADDING * 2) + MOB_COLLISION_WIDTH, UI_PADDING});
    char hp[64];
    sprintf(hp, "hp %d/%d", 20, 20);
    drawText(hp, (Vector2d){(UI_PADDING * 2) + MOB_COLLISION_WIDTH, UI_PADDING + LINE_HEIGHT});
    char mp[64];
    sprintf(mp, "mp %d/%d", 20, 20);
    drawText(mp, (Vector2d){(UI_PADDING * 2) + MOB_COLLISION_WIDTH, UI_PADDING + (LINE_HEIGHT * 2)});

}

void drawMenuView(Player *player) {
    ClearBackground(BLACK);
    drawInGameMenuBox();
    drawPlayer(player);
    drawStats(player);
}

void checkMenuInput(Exploration *exploration) {
    if (IsKeyPressed(KEY_M)) {
        exploration->isMenuOpen = false;
    }
}
