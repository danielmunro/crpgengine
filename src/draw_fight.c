void drawFightBackground(Encounters *encounters) {
    float scale = (float) SCREEN_WIDTH / (float) encounters->background.width;
    DrawTextureEx(encounters->background, (Vector2) {0, 0}, 0, scale, WHITE);
}

void drawFightBeasts(Fight *fight) {
    int width = 0;
    int height = 0;
    for (int i = 0; i < fight->beastCount; i++) {
        const int x = i % 3;
        DrawTextureEx(fight->beasts[i]->image,
                      (Vector2) {(float) width, (float) height},
                      0, SCALE, WHITE);
        width += fight->beasts[i]->image.width;
        if (x > 0 && x % 2 == 0) {
            height += fight->beasts[i]->image.height;
            width = 0;
        }
    }
}

void drawFightPlayer(Player *player) {
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (player->party[i] != NULL) {
            drawAnimation(
                    findAnimation(getPartyLeader(player)->animations, LEFT),
                    (Vector2) {SCREEN_WIDTH * .8, (float) (100 + (MOB_HEIGHT * i))});
        }
    }
}

void drawFightSelectActionMenu(Fight *fight, UIManager *ui) {
    TextBox *t = createTextBox(
            drawActionSelectMenu(),
            getFontStyle(ui->fonts, FONT_STYLE_DEFAULT));
//    drawInMenuWithStyle(t, fight->currentCursor == 0)
    drawInMenu(t, "Attack");
    drawInMenu(t, "Items");
    drawInMenu(t, "Magic");
    drawInMenu(t, "Defend");
    drawInMenu(t, "Run");
}

void drawFightMenu(Fight *fight, UIManager *ui) {
    TextBox *left = createTextBox(
            drawBottomLeftMenu(),
            getFontStyle(ui->fonts, FONT_STYLE_DEFAULT));
    int count = fight->beastCount > MAX_MOB_NAMES_IN_FIGHT ? MAX_MOB_NAMES_IN_FIGHT : fight->beastCount;
    for (int i = 0; i < count; i++) {
        drawInMenu(left, fight->beasts[i]->name);
    }
    free(left);
    Menu *menu = findMenu(ui, MOBILE_SELECT_FIGHT_MENU);
    menu->draw(ui->menuContext);
}

void drawFightView(Encounters *encounters, Fight *fight, UIManager *ui) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawFightBackground(encounters);
    drawFightBeasts(fight);
    drawFightPlayer(fight->player);
    drawFightMenu(fight, ui);
    EndDrawing();
}
