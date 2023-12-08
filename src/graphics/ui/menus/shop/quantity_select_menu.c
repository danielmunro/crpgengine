#include "headers/graphics/ui/menu.h"

int getQuantitySelectCursorLength(const MenuContext *mc) {
    return mc->player->coins / mc->itemToBuy->worth;
}

void drawQuantitySelectMenuScreen(MenuContext *menuContext) {
    TextBox *b = findOrCreateTextBox(
            menuContext,
            QUANTITY_SELECT_BOX,
            ui->textAreas->alert);
    drawMenuRect(b->area);
    drawInMenu(b, "How many would you like to buy?");
    char amount[24];
    sprintf(amount, "Amount: %d", menuContext->quantity);
    drawInMenu(b, amount);
    char costText[24];
    sprintf(costText, "Total cost: %d", (menuContext->quantity) * menuContext->itemToBuy->worth);
    drawInMenu(b, costText);
}

MenuSelectResponse *quantitySelectMenuItemSelected() {
    return createMenuSelectResponse(OPEN_MENU, SHOP_CONFIRM_PURCHASE_MENU);
}
