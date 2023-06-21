void drawPlayer(Player *player) {
    drawAnimation(
            findAnimation(player->mob->animations, DOWN),
            (Vector2){20, 20}
            );
}

void drawStats(Player *player) {
    drawText("playername", (Vector2d){(UI_PADDING * 2) + MOB_COLLISION_WIDTH, UI_PADDING});
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
