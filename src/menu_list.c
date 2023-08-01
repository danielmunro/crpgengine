int getMenuList(Menu *menuList[MAX_MENUS]) {
    Menu *list[] = {
        createMenu(
                PARTY_MENU,
                getPartyMenuCursorLength,
                drawPartyMenuScreen,
                partyMenuItemSelected),
        createMenu(
                ITEMS_MENU,
                getItemsCursorLength,
                drawItemsMenuScreen,
                partyMenuItemSelected),
        createMenu(
                LOAD_MENU,
                getLoadCursorLength,
                drawLoadMenuScreen,
                loadMenuItemSelected),
        createMenu(
                SAVE_MENU,
                getSaveCursorLength,
                drawSaveMenuScreen,
                saveMenuItemSelected),
        createMenu(
                QUIT_MENU,
                getQuitCursorLength,
                drawQuitMenuScreen,
                quitMenuItemSelected),
    };
    int count = sizeof(list) / sizeof(list[0]);
    for (int i = 0; i < count; i++) {
        menuList[i] = list[i];
    }
    return count;
}