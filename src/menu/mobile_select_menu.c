FontStyle *getFontStyleForFightCursor(Mobile *mob, Fonts *fonts, int mobIndex, int loopIndex) {
    if (mobIndex == loopIndex) {
        return fonts->highlight;
    } else if (isReadyForAction(mob)) {
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

void drawActionGauge(float y, float width, Color color) {
    float actionGaugeX = (float) ui->screen->width - ACTION_GAUGE_WIDTH - ui->menu->padding;
//    float actionGaugeY = (float) ui->screen->height - BOTTOM_MENU_HEIGHT + ui->menu->padding + y;
    Rectangle rect = {
            actionGaugeX,
            y,
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
                        ? getFontStyleForFightCursor(mob, mc->fonts, mc->cursorLine, i)
                        : mc->fonts->disable;
        char namePart[255];
        if (mc->fight->defending[i]) {
            sprintf(namePart, "[D] %s", mob->name);
        } else {
            sprintf(namePart, "%s", mob->name);
        }
        drawInMenuWithStyle(textBox,
                            fs,
                            namePart);
        if (mc->cursorLine == i && isReadyForAction(mob)) {
            drawRightCursor(
                    mc->uiSprite,
                    (Vector2) {
                            textBox->area.x,
                            textBox->area.y + (fs->lineHeight * (float) i),
                    });
            if (doDrawDownCursor) {
                int frameHeight = mob->animations[0]->spriteSheet->frameHeight;
                Vector2 playerPosition = getFightPlayerPosition(
                        i,
                        frameHeight);
                drawDownCursor(mc->uiSprite,
                               (Vector2) {
                                       playerPosition.x - 5,
                                       playerPosition.y - (float) frameHeight - 15,
                               });
            }
        }
        drawText(getVital(mob->hp, calculateMobileAttributes(mob).hp),
                (Vector2) {
                        textBox->area.x + HP_X_OFFSET,
                        textBox->area.y + ui->menu->padding + ((float) i * fs->lineHeight)
                },
                mc->fonts->default_);
        drawText(getVital(mob->mana, calculateMobileAttributes(mob).mana),
                (Vector2) {
                        textBox->area.x + MANA_X_OFFSET,
                        textBox->area.y + ui->menu->padding + ((float) i * fs->lineHeight)
                },
                mc->fonts->default_);
        drawActionGauge(
                textBox->area.y + ui->menu->padding + line(i, mc->fonts->default_->lineHeight) + 3,
                ACTION_GAUGE_WIDTH,
                mc->fonts->disable->color);
        drawActionGauge(
                textBox->area.y + ui->menu->padding + line(i, mc->fonts->default_->lineHeight) + 3,
                ACTION_GAUGE_WIDTH * ((float) mob->actionGauge / MAX_ACTION_GAUGE),
                mc->fonts->default_->color);
    }
}

int getMobileSelectMenuCursorLength(MenuContext *menuContext) {
    return menuContext->player->partyCount;
}

TextBox *createMobileSelectTextBox(MenuContext *menuContext) {
    return createTextBox(
            ui->textAreas->bottomMidRight,
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
