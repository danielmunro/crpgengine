#include "../../../../headers/menu.h"
#include "../../../../headers/mobile.h"

FontStyle *getFontStyleForFightCursor(Mobile *mob, Fonts *fonts, int mobIndex, int loopIndex) {
    if (mobIndex == loopIndex) {
        return fonts->highlight;
    } else if (isReadyForAction(mob)) {
        return fonts->default_;
    } else {
        return fonts->disable;
    }
}

void drawActionGauge(Rectangle rect, float percent, Color color) {
    DrawRectangleRounded(
            (Rectangle) {
                    rect.x,
                    rect.y,
                    rect.width * percent,
                    rect.height,
            },
            1,
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
        if (mc->fight->mobsDefending[i]) {
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
        FontStyle *default_ = mc->fonts->default_;
        Attributes calculated = calculateMobileAttributes(mob);
        char hp[MAX_VITALS_LENGTH];
        getVital(mob->hp, calculated.hp, hp);
        drawText(hp,
                 (Vector2) {
                         textBox->area.x + (ui->fightMenu->columns->hp * textBox->area.width),
                         textBox->area.y + ui->menu->padding + ((float) i * fs->lineHeight)
                 },
                 default_);
        char mana[MAX_VITALS_LENGTH];
        getVital(mob->mana, calculated.mana, mana);
        drawText(mana,
                 (Vector2) {
                         textBox->area.x + (ui->fightMenu->columns->mana * textBox->area.width),
                         textBox->area.y + ui->menu->padding + ((float) i * fs->lineHeight)
                 },
                 default_);
        Rectangle rect = (Rectangle) {
                textBox->area.x
                + ui->fightMenu->actionGauge->rect.x
                + (ui->fightMenu->columns->actionGauge * textBox->area.width),
                textBox->area.y
                + ui->menu->padding
                + ui->fightMenu->actionGauge->rect.y
                + line(i, default_->lineHeight),
                ui->fightMenu->actionGauge->rect.width,
                ui->fightMenu->actionGauge->rect.height,
        };
        drawActionGauge(rect, (float) 1.0, mc->fonts->disable->color);
        drawActionGauge(
                rect,
                (float) mob->actionGauge / MAX_ACTION_GAUGE,
                default_->color);
    }
}

int getMobileSelectMenuCursorLength(MenuContext *menuContext) {
    return menuContext->player->partyCount;
}

void drawMobileSelectMenuScreen(MenuContext *menuContext) {
    TextBox *t = findOrCreateTextBox(
            menuContext,
            MOBILE_SELECT_BOX,
            ui->textAreas->bottomMidRight);
    drawMenuRect(t->area);
    drawPlayerFightTopLevel(menuContext, t, true);
}

MenuSelectResponse *mobileSelectMenuItemSelected(MenuContext *menuContext) {
    menuContext->selectedMob = menuContext->player->party[menuContext->cursorLine];
    if (menuContext->selectedMob == NULL || !isReadyForAction(menuContext->selectedMob)) {
        return createMenuSelectResponse(NO_OP, MOBILE_SELECT_FIGHT_MENU);
    }
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
