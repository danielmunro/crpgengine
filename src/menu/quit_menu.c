void drawQuitMenuScreen(Player *player, int cursorLine) {
    Rectangle area = drawAlertBox();
    drawText(
            "Are you sure?",
            (Vector2d) {
                    (int) area.x + UI_PADDING,
                    (int) area.y + UI_PADDING});
    drawText(
            "Yes",
            (Vector2d) {
                    (int) area.x + UI_PADDING,
                    (int) area.y + UI_PADDING + line(2)});
    drawText(
            "No",
            (Vector2d) {
                    (int) area.x + UI_PADDING,
                    (int) area.y + UI_PADDING + line(3)});
    drawText(
            ">",
            (Vector2d) {
                    (int) area.x,
                    (int) area.y + UI_PADDING + line(2 + cursorLine)});
}
