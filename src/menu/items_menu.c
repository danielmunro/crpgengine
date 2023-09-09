TextBox *createItemsTextBox(MenuContext *mc) {
    return createTextBox(
            getFullScreenMenu(),
            mc->fonts->default_,
            ITEMS_BOX);
}

void drawItemsMenuScreen(MenuContext *menuContext) {
    FontStyle *defaultFont = menuContext->fonts->default_;
    TextBox *textBox = findOrCreateTextBox(
            menuContext,
            ITEMS_BOX,
            createItemsTextBox);
    drawMenuRect(textBox->area);
    if (menuContext->itemListCount == 0) {
        ItemList *itemsSeen = calloc(menuContext->player->itemCount, sizeof(ItemList));
        int count = 0;
        for (int i = 0; i < menuContext->player->itemCount; i++) {
            const char *name = menuContext->player->items[i]->name;
            bool found = false;
            for (int j = 0; j < count; j++) {
                if (strcmp(name, itemsSeen[j].item->name) == 0) {
                    itemsSeen[j].amount += 1;
                    found = true;
                    break;
                }
            }
            if (!found) {
                itemsSeen[count] = (ItemList) {
                        menuContext->player->items[i],
                        1,
                };
                count++;
            }
        }
        menuContext->itemList = itemsSeen;
        menuContext->itemListCount = count;
    }
    for (int i = 0; i < menuContext->itemListCount; i++) {
        char buffer[MAX_LINE_BUFFER];
        sprintf(buffer, "(%d) %s", menuContext->itemList[i].amount, menuContext->itemList[i].item->name);
        drawInMenuWithStyle(
                textBox,
                menuContext->itemList[i].item->type == ITEM_TYPE_CONSUMABLE
                    ? menuContext->fonts->default_
                    : menuContext->fonts->disable,
                buffer);
    }
    drawRightCursor(
            menuContext->uiSprite,
            (Vector2) {
                    0,
                    line(menuContext->cursorLine, defaultFont->lineHeight),
            });
}

int getItemsCursorLength(MenuContext *menuContext) {
    return menuContext->itemListCount;
}

MenuSelectResponse *itemMenuItemSelected(MenuContext *menuContext) {
    Item *item = menuContext->itemList[menuContext->cursorLine].item;
    if (item->type == ITEM_TYPE_CONSUMABLE) {
        menuContext->selectedItem = item;
        return createMenuSelectResponse(OPEN_MENU, PARTY_ITEM_CONSUME_MENU);
    }
    return NULL;
}
