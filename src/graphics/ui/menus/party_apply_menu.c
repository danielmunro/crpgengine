int getPartyApplyMenuCursorLength(MenuContext *mc) {
    return mc->player->partyCount;
}

TextBox *createPartyApplyTextBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->right,
            mc->fonts->default_,
            PARTY_APPLY_BOX);
}

void drawPartyApplyPlayer(Mobile *mob, Vector2 pos) {
    Animation *a = findAnimation(mob->animations, DOWN);
    drawAnimation(a, pos);
}

void drawPartyApplyMenuScreen(MenuContext *mc) {
    TextBox *t = findOrCreateTextBox(
            mc,
            PARTY_APPLY_BOX,
            createPartyApplyTextBox);
    drawMenuRect(t->area);
    float quarter = (float) ui->screen->height / 4;
    FontStyle *f = mc->fonts->default_;
    for (int i = 0; i < mc->player->partyCount; i++) {
        Mobile *mob = mc->player->party[i];
        Vector2 pos = (Vector2) {
                ui->menu->padding + t->area.x,
                ui->menu->padding + (quarter * (float) i),
        };
        drawPartyApplyPlayer(mob, pos);
        drawText(
                mob->name,
                (Vector2) {pos.x + ui->menu->padding, pos.y},
                f);
        drawText(
                getVital(mob->hp, calculateMobileAttributes(mob).hp),
                (Vector2) {pos.x + ui->menu->padding, pos.y + line(1, f->lineHeight)},
                f);
        drawText(
                getVital(mob->mana, calculateMobileAttributes(mob).mana),
                (Vector2) {pos.x + ui->menu->padding, pos.y + line(2, f->lineHeight)},
                f);
        if (mc->cursorLine == i) {
            drawRightCursor(
                    mc->uiSprite,
                    (Vector2) {
                            pos.x - ui->menu->padding,
                            pos.y - ui->menu->padding,
                    });
        }
    }
}

MenuSelectResponse *partyApplyMenuItemSelected(MenuContext *mc) {
    if (mc->selectedSpell != NULL) {
        if (!applyCastCost(mc->selectedMob, mc->selectedSpell->cost)) {
            return createMenuSelectResponse(NO_OP, PARTY_APPLY_MENU);
        }
        executeSpellOnMobile(mc->player->party[mc->cursorLine], mc->selectedSpell);
    } else {
        for (int i = 0; i < mc->player->itemCount; i++) {
            if (mc->player->items[i] == mc->selectedItem) {
                applyConsumable(mc->player->party[mc->cursorLine], mc->selectedItem);
                removeItem(mc->player, mc->selectedItem);
                mc->selectedItem = NULL;
                resetItemList(mc);
                break;
            }
        }
    }
    return createMenuSelectResponse(CLOSE_MENU, PARTY_APPLY_MENU);
}
