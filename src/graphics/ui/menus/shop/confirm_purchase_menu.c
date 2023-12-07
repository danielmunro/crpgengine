#include "headers/graphics/ui/menu.h"

int getConfirmPurchaseCursorLength() {
    return 1;
}

void drawConfirmPurchaseMenuScreen(MenuContext *mc) {
    TextBox *b = findOrCreateTextBox(
            mc,
            QUANTITY_SELECT_BOX,
            ui->textAreas->alert);
    drawMenuRect(b->area);
    char purchaseText[64];
    sprintf(
            purchaseText,
            "You bought %s (%d)",
            mc->itemToBuy->item->name,
            mc->quantity);
    drawInMenu(b, purchaseText);
    char purchaseText2[64];
    sprintf(purchaseText2, "for %d coins.", mc->quantity * mc->itemToBuy->worth);
    drawInMenu(b, purchaseText2);
}

MenuSelectResponse *confirmPurchaseMenuItemSelected(MenuContext *mc) {
    for (int i = 0; i < mc->quantity; i++) {
        addItem(mc->player, mc->itemToBuy->item);
    }
    mc->player->coins -= mc->itemToBuy->worth * mc->quantity;
    mc->quantity = 1;
    return createMenuSelectResponse(CLOSE_MENU, SHOP_QUANTITY_SELECT_MENU);
}
