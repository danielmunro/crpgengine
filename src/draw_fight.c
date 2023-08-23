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
//
//void drawFightSelectActionMenu(Fight *fight, UIManager *ui) {
//    TextBox *t = createTextBox(
//            drawActionSelectMenu(),
//            getFontStyle(ui->fonts, FONT_STYLE_DEFAULT));
////    drawInMenuWithStyle(t, fight->currentCursor == 0)
//    drawInMenu(t, "Attack");
//    drawInMenu(t, "Items");
//    drawInMenu(t, "Magic");
//    drawInMenu(t, "Defend");
//    drawInMenu(t, "Run");
//}

void drawFightMenu(FightManager *fm) {
    drawAllMenus(fm->menuContext, fm->menus);
//    findMenu(ui->menus, BEAST_LIST_FIGHT_MENU)->draw(ui->menuContext);
//    findMenu(ui->menus, MOBILE_SELECT_FIGHT_MENU)->draw(ui->menuContext);
//    if (fight->menu == ACTION_SELECT_FIGHT_MENU) {
//        findMenu(ui->menus, ACTION_SELECT_FIGHT_MENU)->draw(ui->menuContext);
//    }
}

void drawFightView(Encounters *encounters, FightManager *fights) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawFightBackground(encounters);
    drawFightBeasts(fights->fight);
    drawFightPlayer(fights->fight->player);
    drawFightMenu(fights);
    EndDrawing();
}
