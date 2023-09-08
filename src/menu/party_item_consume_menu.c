int getPartyItemConsumeMenuCursorLength(MenuContext *mc) {
    return mc->player->partyCount;
}

TextBox *createPartyItemConsumeTextBox(MenuContext *mc) {
    return createTextBox(
            getTopWideMenu(),
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
    float quarter = (float) ui->screen->width / 4;
    for (int i = 0; i < mc->player->partyCount; i++) {
        Vector2 pos = (Vector2) {
                ui->menu->padding + (quarter * (float) i),
                ui->menu->padding,
        };
        drawPartyItemConsumerPlayer(mc->player->party[i], pos);
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
