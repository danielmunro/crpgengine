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

void drawActionGauge(float y, float width, Color color) {
    float actionGaugeX = SCREEN_WIDTH - ACTION_GAUGE_WIDTH - UI_PADDING;
    float actionGaugeY = SCREEN_HEIGHT - BOTTOM_MENU_HEIGHT + UI_PADDING + y;
    Rectangle rect = {
            actionGaugeX,
            actionGaugeY,
            width,
            ACTION_GAUGE_HEIGHT,
    };
    DrawRectangleRounded(
            rect,
            (float) 1,
            4,
            color);
}

FontStyle *getFontStyleForFightCursor(UIManager *ui, Mobile *mob, int mobIndex, int loopIndex) {
    if (mobIndex == loopIndex) {
        return ui->highlightedFont;
    } else if(isReadyForAction(mob)) {
        return ui->defaultFont;
    } else {
        return ui->disabledFont;
    }
}

FontStyle *getFontStyleForHealthLevel(UIManager *ui, float percent) {
    if (percent >= FONT_WARNING_THRESHOLD) {
        return ui->defaultFont;
    } else if (percent >= FONT_DANGER_THRESHOLD) {
        return ui->warningFont;
    } else {
        return ui->dangerFont;
    }
}

void drawStat(UIManager *ui, int amount, float percent, Vector2 vect) {
    char stat[10];
    sprintf(stat, "%d", amount);
    FontStyle *fs = getFontStyleForHealthLevel(ui,percent);
    drawText(stat,vect,fs);
}

void drawPlayerFightTopLevel(Fight *fight, TextBox *textBox, UIManager *ui) {
    for (int i = 0; i < fight->player->partyCount; i++) {
        Mobile *mob = fight->player->party[i];
        FontStyle *fs = getFontStyleForFightCursor(ui, mob, fight->cursors[fight->menu], i);
        drawInMenuWithStyle(textBox,
                fs,
                mob->name);
        if (fight->cursors[fight->menu] == i) {
            Spritesheet *spritesheet = findSpritesheetByName(ui->sprites, SPRITESHEET_NAME_UI);
            drawImageFromSprite(spritesheet, (Vector2) {
                    textBox->area.x + FIGHT_CURSOR_X_OFFSET,
                    textBox->area.y + (float) (LINE_HEIGHT * i) + FIGHT_CURSOR_Y_OFFSET,
            }, CURSOR_INDEX);
        }
        drawStat(ui,
                mob->hp,
                (float) mob->hp / (float) calculateAttributes(mob).hp,
                (Vector2) {
                        textBox->area.x + HP_X_OFFSET,
                        textBox->area.y + UI_PADDING + (float) (i * LINE_HEIGHT)
                });
        drawStat(ui,
                mob->mana,
                (float) mob->mana / (float) calculateAttributes(mob).mana,
                (Vector2) {
                        textBox->area.x + MANA_X_OFFSET,
                        textBox->area.y + UI_PADDING + (float) (i * LINE_HEIGHT)
                });
        drawActionGauge(
                (float) (i * LINE_HEIGHT),
                ACTION_GAUGE_WIDTH,
                ui->disabledFont->color);
        drawActionGauge(
                (float) (i * LINE_HEIGHT),
                ACTION_GAUGE_WIDTH * ((float) fight->player->party[i]->actionGauge / MAX_ACTION_GAUGE),
                ui->defaultFont->color);
    }
}

void drawFightSelectActionMenu(Fight *fight, UIManager *ui) {
    TextBox *t = createTextBox(drawActionSelectMenu(), ui->defaultFont);
//    drawInMenuWithStyle(t, fight->currentCursor == 0)
    drawInMenu(t, "Attack");
    drawInMenu(t, "Items");
    drawInMenu(t, "Magic");
    drawInMenu(t, "Defend");
    drawInMenu(t, "Run");
}

void drawFightMenu(Fight *fight, UIManager *ui) {
    TextBox *left = createTextBox(drawBottomLeftMenu(), ui->defaultFont);
    TextBox *right = createTextBox(drawBottomRightMenu(), ui->defaultFont);
    int count = fight->beastCount > MAX_MOB_NAMES_IN_FIGHT ? MAX_MOB_NAMES_IN_FIGHT : fight->beastCount;
    for (int i = 0; i < count; i++) {
        drawInMenu(left, fight->beasts[i]->name);
    }
    drawPlayerFightTopLevel(fight, right, ui);
    if (fight->menu == FIGHT_MENU_ACTION_SELECT) {
        drawFightSelectActionMenu(fight, ui);
    }
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
