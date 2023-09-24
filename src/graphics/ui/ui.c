typedef struct {
    int width;
    int height;
    float scale;
    int targetFrameRate;
} ScreenConfig;

typedef struct {
    Color top;
    Color bottom;
} VerticalGradientConfig;

typedef struct {
    int lineThickness;
    float roundness;
    Color color;
} BorderConfig;

typedef struct {
    Rectangle rect;
} ActionGaugeConfig;

typedef struct {
    float name;
    float hp;
    float mana;
    float actionGauge;
} ColumnsConfig;

typedef struct {
    MenuStyle style;
    VerticalGradientConfig *verticalGradient;
    BorderConfig *border;
    float padding;
} MenuConfig;

typedef struct {
    Rectangle small;
    Rectangle medium;
    Rectangle full;
    Rectangle bottom;
    Rectangle right;
    Rectangle bottomLeft;
    Rectangle bottomMidRight;
    Rectangle bottomMid;
} TextAreasConfig;

typedef struct {
    ActionGaugeConfig *actionGauge;
    ColumnsConfig *columns;
} FightMenuConfig;

typedef struct {
    ScreenConfig *screen;
    MenuConfig *menu;
    TextAreasConfig *textAreas;
    FightMenuConfig *fightMenu;
} UIConfig;

UIConfig *ui;

Rectangle findTextAreaRect(TextAreaType type, TextAreaData *areas, int areaCount) {
    for (int i = 0; i < areaCount; i++) {
        if (strcmp(areas[i].type, TextAreaTypes[type]) == 0) {
            return (Rectangle) {
                    (float) ui->screen->width * areas[i].x,
                    (float) ui->screen->height * areas[i].y,
                    (float) ui->screen->width * areas[i].width,
                    (float) ui->screen->height * areas[i].height,
            };
        }
    }
    addError("unknown text area: %d", type);
    exit(EXIT_UNKNOWN_TEXT_AREA);
}

void createUIConfig(UIData *data) {
    ui = malloc(sizeof(UIConfig));

    ui->screen = malloc(sizeof(ScreenConfig));
    ui->screen->height = data->screen->height;
    ui->screen->width = data->screen->width;
    ui->screen->scale = data->screen->scale;
    ui->screen->targetFrameRate = data->screen->targetFrameRate;

    ui->menu = malloc(sizeof(MenuConfig));
    ui->menu->style = getMenuStyleFromString(data->menu->style);
    ui->menu->padding = data->menu->padding;
    ui->menu->verticalGradient = NULL;
    if (ui->menu->style == VERTICAL_GRADIENT) {
        ui->menu->verticalGradient = malloc(sizeof(VerticalGradientConfig));
        ui->menu->verticalGradient->top = getColorFromString(data->menu->verticalGradient->topColor);
        ui->menu->verticalGradient->bottom = getColorFromString(data->menu->verticalGradient->bottomColor);
    }

    ui->menu->border = malloc(sizeof(BorderConfig));
    ui->menu->border->lineThickness = data->menu->border->lineThickness;
    ui->menu->border->roundness = data->menu->border->roundness;
    ui->menu->border->color = getColorFromString(data->menu->border->color);

    ui->fightMenu = malloc(sizeof(FightMenuConfig));
    ui->fightMenu->columns = malloc(sizeof(ColumnsConfig));
    ui->fightMenu->columns->name = data->fightMenu->columns->name;
    ui->fightMenu->columns->hp = data->fightMenu->columns->hp;
    ui->fightMenu->columns->mana = data->fightMenu->columns->mana;
    ui->fightMenu->columns->actionGauge = data->fightMenu->columns->actionGauge;

    ui->fightMenu->actionGauge = malloc(sizeof(ActionGaugeConfig));
    ui->fightMenu->actionGauge->rect = (Rectangle) {
            data->fightMenu->actionGauge->x,
            data->fightMenu->actionGauge->y,
            data->fightMenu->actionGauge->width,
            data->fightMenu->actionGauge->height,
    };

    ui->textAreas = malloc(sizeof(TextAreasConfig));
    ui->textAreas->small = findTextAreaRect(
            TEXT_AREA_SMALL,
            data->textAreas,
            data->textAreasCount);
    ui->textAreas->medium = findTextAreaRect(
            TEXT_AREA_MEDIUM,
            data->textAreas,
            data->textAreasCount);
    ui->textAreas->full = findTextAreaRect(
            TEXT_AREA_FULL,
            data->textAreas,
            data->textAreasCount);
    ui->textAreas->bottom = findTextAreaRect(
            TEXT_AREA_BOTTOM,
            data->textAreas,
            data->textAreasCount);
    ui->textAreas->right = findTextAreaRect(
            TEXT_AREA_RIGHT,
            data->textAreas,
            data->textAreasCount);
    ui->textAreas->bottomLeft = findTextAreaRect(
            TEXT_AREA_BOTTOM_LEFT,
            data->textAreas,
            data->textAreasCount);
    ui->textAreas->bottomMidRight = findTextAreaRect(
            TEXT_AREA_BOTTOM_MID_RIGHT,
            data->textAreas,
            data->textAreasCount);
    ui->textAreas->bottomMid = findTextAreaRect(
            TEXT_AREA_BOTTOM_MID,
            data->textAreas,
            data->textAreasCount);
}

typedef struct {
    Rectangle area;
    FontStyle *fontStyle;
    TextBoxLabel label;
    int cursor;
} TextBox;

TextBox *createTextBox(Rectangle area, FontStyle *fontStyle, TextBoxLabel label) {
    TextBox *textBox = malloc(sizeof(TextBox));
    textBox->area = area;
    textBox->fontStyle = fontStyle;
    textBox->label = label;
    textBox->cursor = 0;
    return textBox;
}

void drawText(const char *message, Vector2 position, FontStyle *fontStyle) {
    DrawTextEx(
            fontStyle->font,
            message,
            position,
            fontStyle->size,
            fontStyle->spacing,
            fontStyle->color);
}

void drawLineInArea(const char *message, Rectangle area, int lineNumber, FontStyle *font) {
    drawText(
            message,
            (Vector2) {
                    area.x + ui->menu->padding,
                    area.y + ui->menu->padding + (font->lineHeight * (float) lineNumber)
            }, font);
}

void drawTextInArea(const char *message, Rectangle area, FontStyle *font) {
    char m[MAX_MESSAGE_BUFFER];
    char buffer[MAX_LINE_BUFFER];
    strcpy(m, message);
    strcpy(buffer, "");
    char *word = strtok(m, " ");
    int line = 0;
    char test[MAX_LINE_BUFFER];
    while (word != NULL) {
        strcpy(test, buffer);
        if (strcmp(test, "") != 0) {
            strcat(test, " ");
        }
        strcat(test, word);
        Vector2 testArea = MeasureTextEx(font->font, test, font->size, 1);
        if (testArea.x > area.width - ui->menu->padding * 2) {
            drawLineInArea(buffer, area, line, font);
            line++;
            strcpy(buffer, word);
        } else {
            strcpy(buffer, test);
        }
        word = strtok(NULL, " ");
    }
    if (strcmp(buffer, "") != 0) {
        drawLineInArea(buffer, area, line, font);
    }
    free(word);
}

void drawMenuRect(Rectangle rect) {
    float t = (float) ui->menu->border->lineThickness;
    float t2 = t * 2;
    if (ui->menu->style == VERTICAL_GRADIENT) {
        DrawRectangleGradientV(
                (int) (rect.x + t),
                (int) (rect.y + t),
                (int) (rect.width - t2),
                (int) (rect.height - t2),
                ui->menu->verticalGradient->top,
                ui->menu->verticalGradient->bottom);
    }
    DrawRectangleRoundedLines(
            (Rectangle) {
                    rect.x + t,
                    rect.y + t,
                    rect.width - t2,
                    rect.height - t2},
            ui->menu->border->roundness,
            4,
            t,
            ui->menu->border->color);
}

float line(int line, float lineHeight) {
    return (float) line * lineHeight;
}

void drawScrollableInMenuWithStyle(TextBox *tb, FontStyle *fs, const char *text, int menuCursor) {
    float offset = getScrollOffset(fs->lineHeight, menuCursor, tb->area.height);
    float y = tb->area.y
              + line(tb->cursor, fs->lineHeight)
              + ui->menu->padding
              - offset;
    if (y >= tb->area.y) {
        drawText(text, (Vector2) {
                tb->area.x + ui->menu->padding,
                y,
        }, fs);
    }
    tb->cursor++;
}

void drawInMenuWithStyle(TextBox *tb, FontStyle *fs, const char *text) {
    drawScrollableInMenuWithStyle(tb, fs, text, 0);
}

void drawScrollableInMenu(TextBox *textBox, const char *text, int cursorLine) {
    drawScrollableInMenuWithStyle(textBox, textBox->fontStyle, text, cursorLine);
}

void drawInMenu(TextBox *textBox, const char *text) {
    drawInMenuWithStyle(textBox, textBox->fontStyle, text);
}
