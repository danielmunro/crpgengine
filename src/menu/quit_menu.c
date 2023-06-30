void drawQuitMenuScreen(Player *player, int cursorLine) {
    Rectangle rect = drawAlertBox();
    rect.x += UI_PADDING;
    rect.y += UI_PADDING;
    TextBox *textBox = createTextBox(fromRectangle(rect));
    drawInTextBox(textBox, "Are you sure?");
    drawInTextBox(textBox, "");
    for (int i = 0; i < QUIT_MENU_ITEM_COUNT; i++) {
        drawInTextBox(textBox, QuitMenuItems[i]);
    }
    drawText(
            ">",
            (Vector2D) {
                    (int) rect.x - UI_PADDING,
                    (int) rect.y + line(2 + cursorLine)
            }
        );
}
