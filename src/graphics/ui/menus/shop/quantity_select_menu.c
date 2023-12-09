#include "headers/graphics/ui/menu.h"

int getQuantitySelectCursorLength(const MenuContext *mc) {
    return mc->player->coins / mc->itemToBuy->worth;
}

void drawQuantitySelectMenuScreen(MenuContext *mc) {
    TextBox *b = findOrCreateTextBox(
            mc,
            QUANTITY_SELECT_BOX,
            ui->textAreas->alert);
    drawMenuRect(b->area);
    drawInMenu(b, "How many would you like to buy?");
    char amount[24];
    sprintf(amount, "%s x %d", mc->itemToBuy->item->name, mc->quantity);
    drawInMenu(b, amount);
    TextBox *b2 = findOrCreateTextBox(
            mc,
            SHOP_QUANTITY_COSTS_BOX,
            ui->textAreas->alertRight);
    char costText[24];
    sprintf(costText, "= %d", (mc->quantity) * mc->itemToBuy->worth);
    drawInMenu(b2, "");
    drawInMenu(b2, costText);
}

MenuSelectResponse *quantitySelectMenuItemSelected() {
    return createMenuSelectResponse(OPEN_MENU, SHOP_CONFIRM_PURCHASE_MENU);
}
