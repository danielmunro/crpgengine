FontStyle *getFontStyleForFightCursor(Mobile *mob, FontStyle **fonts, int mobIndex, int loopIndex) {
    if (mobIndex == loopIndex) {
        return getFontStyle(fonts, FONT_STYLE_HIGHLIGHT);
    } else if(isReadyForAction(mob)) {
        return getFontStyle(fonts, FONT_STYLE_DEFAULT);
    } else {
        return getFontStyle(fonts, FONT_STYLE_DISABLE);
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

void drawPlayerFightTopLevel(MenuContext *mc, TextBox *textBox, bool doDrawDownCursor) {
    for (int i = 0; i < mc->fight->player->partyCount; i++) {
        Mobile *mob = mc->fight->player->party[i];
        FontStyle *fs = isReadyForAction(mob)
                ? getFontStyleForFightCursor(mob,  mc->fonts, mc->cursorLine, i)
                : getFontStyle(mc->fonts, FONT_STYLE_DISABLE);
        drawInMenuWithStyle(textBox,
                            fs,
                            mob->name);
        if (mc->cursorLine == i && isReadyForAction(mob)) {
            drawRightCursor(
                    mc->menuSprite,
                    (Vector2) {
                            textBox->area.x,
                            textBox->area.y + (fs->lineHeight * (float) i),
                    });
            if (doDrawDownCursor) {
                Vector2 playerPosition = getFightPlayerPosition(i);
                int frameHeight = mob->animations[0]->spriteSheet->frameHeight;
                drawDownCursor(mc->menuSprite,
                               (Vector2) {
                                       playerPosition.x - 5,
                                       playerPosition.y - (float) frameHeight - 15,
                               });
            }
        }
        drawStat(mc->fonts,
                 mob->hp,
                 (float) mob->hp / (float) calculateMobileAttributes(mob).hp,
                 (Vector2) {
                         textBox->area.x + HP_X_OFFSET,
                         textBox->area.y + UI_PADDING + ((float) i * fs->lineHeight)
                 });
        drawStat(mc->fonts,
                 mob->mana,
                 (float) mob->mana / (float) calculateMobileAttributes(mob).mana,
                 (Vector2) {
                         textBox->area.x + MANA_X_OFFSET,
                         textBox->area.y + UI_PADDING + ((float) i * fs->lineHeight)
                 });
        drawActionGauge(
                ((float) i * fs->lineHeight),
                ACTION_GAUGE_WIDTH,
                getFontStyle(mc->fonts, FONT_STYLE_DISABLE)->color);
        drawActionGauge(
                ((float) i * fs->lineHeight),
                ACTION_GAUGE_WIDTH * ((float) mob->actionGauge / MAX_ACTION_GAUGE),
                getFontStyle(mc->fonts, FONT_STYLE_DEFAULT)->color);
    }
}

int getMobileSelectMenuCursorLength(MenuContext *menuContext) {
    return menuContext->player->partyCount;
}

TextBox *createMobileSelectTextBox(MenuContext *menuContext) {
    return createTextBox(
            drawBottomRightMenu(),
            getFontStyle(menuContext->fonts, FONT_STYLE_DEFAULT), MOBILE_SELECT_BOX);
}

void drawMobileSelectMenuScreen(MenuContext *menuContext) {
    drawBottomRightMenu();
    TextBox *t = findOrCreateTextBox(menuContext, MOBILE_SELECT_BOX, createMobileSelectTextBox);
    drawPlayerFightTopLevel(menuContext, t, true);
}

MenuSelectResponse *mobileSelectMenuItemSelected(MenuContext *menuContext) {
    menuContext->selectedMob = menuContext->player->party[menuContext->cursorLine];
    return createMenuSelectResponse(OPEN_MENU, ACTION_SELECT_FIGHT_MENU);
}

int getNextMobileSelectCursorPosition(MenuContext *mc) {
    Player *p = mc->player;
    for (int i = mc->cursorLine + 1; i < mc->player->partyCount; i++) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    for (int i = 0; i < mc->cursorLine; i++) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    return mc->cursorLine;
}

int getPreviousMobileSelectCursorPosition(MenuContext *mc) {
    Player *p = mc->player;
    for (int i = mc->cursorLine - 1; i >= 0; i--) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    for (int i = mc->player->partyCount - 1; i > mc->cursorLine; i--) {
        if (isReadyForAction(p->party[i])) {
            return i;
        }
    }
    return mc->cursorLine;
}
