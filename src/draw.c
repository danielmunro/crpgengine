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

void drawActionGauge(Rectangle rect, Color color) {
    DrawRectangleRounded(
            rect,
            (float) 1,
            4,
            color);
}

void drawPlayerFightTopLevel(Fight *fight, TextBox *textBox, UIManager *ui) {
    for (int i = 0; i < fight->player->partyCount; i++) {
        Mobile *mob = fight->player->party[i];
        FontStyle *fs;
        if (fight->cursors[FIGHT_CURSOR_MAIN] == i) {
            fs = ui->highlightedFont;
        } else if(isReadyForAction(mob)) {
            fs = ui->defaultFont;
        } else {
            fs = ui->disabledFont;
        }
        drawInMenuWithStyle(
                textBox,
                fs,
                mob->name);
        if (fight->cursors[FIGHT_CURSOR_MAIN] == i) {
            DrawRectangle(
                    (int) textBox->area.x + UI_PADDING,
                    (int) textBox->area.y + (LINE_HEIGHT * (i + 1)),
                    86,
                    2,
                    ui->highlightedFont->color);
        }
        char hp[10];
        sprintf(hp, "%d", fight->player->party[i]->hp);
        if ((float) mob->hp / (float) calculateAttributes(mob).hp >= FONT_WARNING_THRESHOLD) {
            fs = ui->defaultFont;
        } else if ((float) mob->hp / (float) calculateAttributes(mob).hp >= FONT_DANGER_THRESHOLD) {
            fs = ui->warningFont;
        } else {
            fs = ui->dangerFont;
        }
        drawText(
                hp,
                (Vector2) {
                        textBox->area.x + HP_X_OFFSET,
                        textBox->area.y + UI_PADDING + (float) (i * LINE_HEIGHT)
                },
                fs);
        char mana[10];
        if ((float) mob->mana / (float) calculateAttributes(mob).mana >= FONT_WARNING_THRESHOLD) {
            fs = ui->defaultFont;
        } else if ((float) mob->mana / (float) calculateAttributes(mob).mana >= FONT_DANGER_THRESHOLD) {
            fs = ui->warningFont;
        } else {
            fs = ui->dangerFont;
        }
        sprintf(mana, "%d", fight->player->party[i]->mana);
        drawText(
                mana,
                (Vector2) {
                        textBox->area.x + MANA_X_OFFSET,
                        textBox->area.y + UI_PADDING + (float) (i * LINE_HEIGHT)
                },
                fs);
        drawActionGauge(
                (Rectangle) {
                        textBox->area.x + ACTION_GAUGE_X_OFFSET,
                        textBox->area.y + ACTION_GAUGE_Y_OFFSET + (float) (i * LINE_HEIGHT),
                        ACTION_GAUGE_WIDTH,
                        ACTION_GAUGE_HEIGHT,
                },
                ui->disabledFont->color);
        drawActionGauge(
                (Rectangle) {
                        textBox->area.x + ACTION_GAUGE_X_OFFSET,
                        textBox->area.y + ACTION_GAUGE_Y_OFFSET + (float) (i * LINE_HEIGHT),
                        ACTION_GAUGE_WIDTH * ((float) fight->player->party[i]->actionGauge / MAX_ACTION_GAUGE),
                        ACTION_GAUGE_HEIGHT,
                },
                ui->defaultFont->color);
    }
}

void drawFightMenu(Fight *fight, UIManager *ui) {
    TextBox *left = createTextBox(drawBottomLeftMenu(), ui->defaultFont);
    TextBox *right = createTextBox(drawBottomRightMenu(), ui->defaultFont);
    int count = fight->beastCount > MAX_MOB_NAMES_IN_FIGHT ? MAX_MOB_NAMES_IN_FIGHT : fight->beastCount;
    for (int i = 0; i < count; i++) {
        drawInMenu(left, fight->beasts[i]->name);
    }
    drawPlayerFightTopLevel(fight, right, ui);
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
