FontStyle *getFontStyleForFightCursor(Mobile *mob, FontStyle **fonts, int mobIndex, int loopIndex) {
    if (mobIndex == loopIndex) {
        return getFontStyle(fonts, FONT_STYLE_HIGHLIGHTED);
    } else if(isReadyForAction(mob)) {
        return getFontStyle(fonts, FONT_STYLE_DEFAULT);
    } else {
        return getFontStyle(fonts, FONT_STYLE_DISABLED);
    }
}

FontStyle *getFontStyleForHealthLevel(FontStyle **fonts, float percent) {
    if (percent >= FONT_WARNING_THRESHOLD) {
        return getFontStyle(fonts, FONT_STYLE_DEFAULT);
    } else if (percent >= FONT_DANGER_THRESHOLD) {
        return getFontStyle(fonts, FONT_STYLE_WARNING);
    } else {
        return getFontStyle(fonts, FONT_STYLE_DANGER);
    }
}

void drawStat(FontStyle **fonts, int amount, float percent, Vector2 vect) {
    char stat[10];
    sprintf(stat, "%d", amount);
    FontStyle *fs = getFontStyleForHealthLevel(fonts, percent);
    drawText(stat,vect,fs);
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

void drawPlayerFightTopLevel(Fight *fight, TextBox *textBox, FontStyle **fonts) {
    for (int i = 0; i < fight->player->partyCount; i++) {
        Mobile *mob = fight->player->party[i];
        FontStyle *fs = getFontStyleForFightCursor(mob, fonts, fight->cursors[fight->menu], i);
        drawInMenuWithStyle(textBox,
                            fs,
                            mob->name);
        if (fight->cursors[fight->menu] == i) {
            drawImageFromSprite(fight->menuSprite, (Vector2) {
                    textBox->area.x + FIGHT_CURSOR_X_OFFSET,
                    textBox->area.y + (float) (LINE_HEIGHT * i) + FIGHT_CURSOR_Y_OFFSET,
            }, CURSOR_INDEX);
        }
        drawStat(fonts,
                 mob->hp,
                 (float) mob->hp / (float) calculateAttributes(mob).hp,
                 (Vector2) {
                         textBox->area.x + HP_X_OFFSET,
                         textBox->area.y + UI_PADDING + (float) (i * LINE_HEIGHT)
                 });
        drawStat(fonts,
                 mob->mana,
                 (float) mob->mana / (float) calculateAttributes(mob).mana,
                 (Vector2) {
                         textBox->area.x + MANA_X_OFFSET,
                         textBox->area.y + UI_PADDING + (float) (i * LINE_HEIGHT)
                 });
        drawActionGauge(
                (float) (i * LINE_HEIGHT),
                ACTION_GAUGE_WIDTH,
                DISABLED_COLOR);
        drawActionGauge(
                (float) (i * LINE_HEIGHT),
                ACTION_GAUGE_WIDTH * ((float) fight->player->party[i]->actionGauge / MAX_ACTION_GAUGE),
                DEFAULT_COLOR);
    }
}

int getMobileSelectFightMenuCursorLength(MenuContext *menuContext) {
    return MAX_PARTY_SIZE;
}

void drawMobileSelectFightMenuScreen(MenuContext *menuContext) {
    TextBox *right = createTextBox(
            drawBottomRightMenu(),
            getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT));
//    drawPlayerFightTopLevel(menuContext->fight, right, menuContext->fonts);
    free(right);
}

MenuSelectResponse *mobileSelectFightMenuItemSelected(MenuContext *menuContext) {
    return createMenuSelectResponse(OPEN_MENU, ACTION_SELECT_FIGHT_MENU);
}
