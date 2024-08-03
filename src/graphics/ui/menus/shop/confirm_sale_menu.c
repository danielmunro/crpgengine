#include "headers/graphics/ui/menu.h"

int getConfirmSaleCursorLength() {
    return 0;
}

void drawConfirmSaleMenuScreen(MenuContext *mc) {
    TextBox *b = findOrCreateTextBox(
            mc,
            QUANTITY_SELL_BOX,
            mc->context->ui->textAreas->alert);
    drawMenuRect(mc->context->ui->menu, b->area);
    char purchaseText[64];
    sprintf(
            purchaseText,
            "You sold %s (%d)",
            mc->itemToSell->name,
            mc->quantity);
    drawInMenu(b, purchaseText);
    char purchaseText2[64];
    sprintf(purchaseText2, "for %d coins.", mc->quantity * mc->itemToSell->worth);
    drawInMenu(b, purchaseText2);
}

MenuSelectResponse *confirmSaleMenuItemSelected(MenuContext *mc) {
    for (int i = 0; i < mc->quantity; i++) {
        removeItem(mc->player, mc->itemToSell);
    }
    mc->player->coins += mc->itemToSell->worth * mc->quantity;
    int quantitySold = mc->quantity;
    mc->quantity = 1;
    mc->itemListCount = 0;
    if (quantitySold == 1) {
        return createMenuSelectResponse(
                RESPONSE_TYPE_CLOSE_MENU,
                SHOP_CONFIRM_SALE_MENU);
    }
    return createMenuSelectResponse(
            RESPONSE_TYPE_CLOSE_MENU,
            SHOP_QUANTITY_SELL_MENU);
}
