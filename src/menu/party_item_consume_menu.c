int getPartyItemConsumeMenuCursorLength(MenuContext *mc) {
    return mc->player->partyCount;
}

TextBox *createPartyItemConsumeTextBox(MenuContext *mc) {
    return createTextBox(
            ui->textAreas->right,
            mc->fonts->default_,
            PARTY_ITEM_CONSUME_BOX);
}

void drawPartyItemConsumerPlayer(Mobile *mob, Vector2 pos) {
    Animation *a = findAnimation(mob->animations, DOWN);
    drawAnimation(a, pos);
}

void drawPartyItemConsumeMenuScreen(MenuContext *mc) {
    TextBox *t = findOrCreateTextBox(
            mc,
            PARTY_ITEM_CONSUME_BOX,
            createPartyItemConsumeTextBox);
    drawMenuRect(t->area);
    float quarter = (float) ui->screen->height / 4;
    FontStyle *f = mc->fonts->default_;
    for (int i = 0; i < mc->player->partyCount; i++) {
        Mobile *mob = mc->player->party[i];
        Vector2 pos = (Vector2) {
                ui->menu->padding + t->area.x,
                ui->menu->padding + (quarter * (float) i),
        };
        drawPartyItemConsumerPlayer(mob, pos);
        drawText(
                mob->name,
                (Vector2) {pos.x + 30, pos.y },
                f);
        drawText(
                getVital(mob->hp, calculateMobileAttributes(mob).hp),
                (Vector2) {pos.x + 30, pos.y + line(1, f->lineHeight) },
                f);
        drawText(
                getVital(mob->mana, calculateMobileAttributes(mob).mana),
                (Vector2) {pos.x + 30, pos.y + line(2, f->lineHeight) },
                f);
        if (mc->cursorLine == i) {
            drawUpCursor(
                    mc->uiSprite,
                    (Vector2) {
                        pos.x - (float) mc->uiSprite->sprite->frameWidth / 4,
                        pos.y + ui->menu->padding,
                    });
        }
    }
}

MenuSelectResponse *partyItemConsumeMenuItemSelected(MenuContext *mc) {
    for (int i = 0; i < mc->player->itemCount; i++) {
        if (mc->player->items[i] == mc->selectedItem) {
            applyConsumable(mc->player->party[mc->cursorLine], mc->selectedItem);
            removeItem(mc->player, mc->selectedItem);
            mc->selectedItem = NULL;
            resetItemList(mc);
            break;
        }
    }
    return createMenuSelectResponse(CLOSE_MENU, PARTY_ITEM_CONSUME_MENU);
}
