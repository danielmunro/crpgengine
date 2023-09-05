FontStyle *getFontStyleForFightCursor(Mobile *mob, Fonts *fonts, int mobIndex, int loopIndex) {
    if (mobIndex == loopIndex) {
        return fonts->highlight;
    } else if(isReadyForAction(mob)) {
        return fonts->default_;
    } else {
        return fonts->disable;
    }
}

FontStyle *getFontStyleForHealthLevel(Fonts *fonts, float percent) {
    if (percent >= FONT_WARNING_THRESHOLD) {
        return fonts->default_;
    } else if (percent >= FONT_DANGER_THRESHOLD) {
        return fonts->warning;
    } else {
        return fonts->danger;
    }
}

void drawStat(Fonts *fonts, int amount, float percent, Vector2 vect) {
    char stat[10];
    sprintf(stat, "%d", amount);
    FontStyle *fs = getFontStyleForHealthLevel(fonts, percent);
    drawText(stat,vect,fs);
}

void drawActionGauge(float y, float width, Color color) {
    float actionGaugeX = (float) ui->screen->width - ACTION_GAUGE_WIDTH - UI_PADDING;
    float actionGaugeY = (float) ui->screen->height - BOTTOM_MENU_HEIGHT + UI_PADDING + y;
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
                : mc->fonts->disable;
        drawInMenuWithStyle(textBox,
                            fs,
                            mob->name);
        if (mc->cursorLine == i && isReadyForAction(mob)) {
            drawRightCursor(
                    mc->uiSprite,
                    (Vector2) {
                            textBox->area.x,
                            textBox->area.y + (fs->lineHeight * (float) i),
                    });
            if (doDrawDownCursor) {
                Vector2 playerPosition = getFightPlayerPosition(i);
                int frameHeight = mob->animations[0]->spriteSheet->frameHeight;
                drawDownCursor(mc->uiSprite,
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
                mc->fonts->disable->color);
        drawActionGauge(
                ((float) i * fs->lineHeight),
                ACTION_GAUGE_WIDTH * ((float) mob->actionGauge / MAX_ACTION_GAUGE),
                mc->fonts->default_->color);
    }
}

int getMobileSelectMenuCursorLength(MenuContext *menuContext) {
    return menuContext->player->partyCount;
}

TextBox *createMobileSelectTextBox(MenuContext *menuContext) {
    return createTextBox(
            getBottomRightMenu(),
            menuContext->fonts->default_, MOBILE_SELECT_BOX);
}

void drawMobileSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MOBILE_SELECT_BOX,
            createMobileSelectTextBox);
    drawMenuRect(t->area);
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
