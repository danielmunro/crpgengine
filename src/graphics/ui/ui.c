#include <string.h>
#include "src/headers/graphics/ui/menu_type.h"
#include "headers/util/util.h"

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
    Rectangle alert;
    Rectangle alertRight;
    Rectangle small;
    Rectangle medium;
    Rectangle full;
    Rectangle bottom;
    Rectangle right;
    Rectangle left;
    Rectangle bottomLeft;
    Rectangle bottomMidRight;
    Rectangle bottomMid;
    Rectangle midRight;
    Rectangle mediumRight;
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

typedef struct {
    Rectangle area;
    FontStyle *fontStyle;
    TextBoxLabel label;
    int cursor;
} TextBox;

typedef struct {
    const char *message;
    Rectangle area;
    const FontStyle *font;
    double timeStarted;
    double timeElapsed;
} Dialog;

UIConfig *ui;

Dialog *createDialog(const char *message, Rectangle area, const FontStyle *font) {
    Dialog *dialog = malloc(sizeof(Dialog));
    dialog->message = message;
    dialog->area = area;
    dialog->font = font;
    dialog->timeStarted = getTimeInMS();
    dialog->timeElapsed = 0;
    return dialog;
}

MenuStyle getMenuStyleFromString(const char *style) {
    int count = sizeof(MenuStyles) / sizeof(MenuStyles[0]);
    for (int i = 0; i < count; i++) {
        if (strcmp(style, MenuStyles[i]) == 0) {
            return i;
        }
    }
    fprintf(stderr, "cannot get menu style :: %s", style);
    exit(ConfigurationErrorMenuStyleNotDefined);
}

Rectangle getScreenRectangle(TextAreaData *data, const UserConfig *userConfig) {
    return (Rectangle) {
            data->x * (float) userConfig->resolution.width,
        data->y * (float) userConfig->resolution.height,
        data->width * (float) userConfig->resolution.width,
        data->height * (float) userConfig->resolution.height,
    };
}

void createUIConfig(UIData *data, const UserConfig *userConfig) {
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
    ui->textAreas->alert = getScreenRectangle(data->textAreas->alert, userConfig);
    ui->textAreas->alertRight = getScreenRectangle(data->textAreas->alertRight, userConfig);
    ui->textAreas->small = getScreenRectangle(data->textAreas->small, userConfig);
    ui->textAreas->medium = getScreenRectangle(data->textAreas->medium, userConfig);
    ui->textAreas->mediumRight = getScreenRectangle(data->textAreas->mediumRight, userConfig);
    ui->textAreas->full = getScreenRectangle(data->textAreas->full, userConfig);
    ui->textAreas->bottom = getScreenRectangle(data->textAreas->bottom, userConfig);
    ui->textAreas->left = getScreenRectangle(data->textAreas->left, userConfig);
    ui->textAreas->right = getScreenRectangle(data->textAreas->right, userConfig);
    ui->textAreas->bottomLeft = getScreenRectangle(data->textAreas->bottomLeft, userConfig);
    ui->textAreas->bottomMidRight = getScreenRectangle(data->textAreas->bottomMidRight, userConfig);
    ui->textAreas->bottomMid = getScreenRectangle(data->textAreas->bottomMidRight, userConfig);
    ui->textAreas->midRight = getScreenRectangle(data->textAreas->midRight, userConfig);
}

TextBox *createTextBox(Rectangle area, FontStyle *fontStyle, TextBoxLabel label) {
    TextBox *textBox = malloc(sizeof(TextBox));
    textBox->area = area;
    textBox->fontStyle = fontStyle;
    textBox->label = label;
    textBox->cursor = 0;
    return textBox;
}

void drawText(const char *message, Vector2 position, const FontStyle *fontStyle) {
    DrawTextEx(
            fontStyle->font,
            message,
            position,
            fontStyle->size,
            fontStyle->spacing,
            fontStyle->color);
}

void drawLineInArea(const char *message, Rectangle area, int lineNumber, const FontStyle *font) {
    drawText(
            message,
            (Vector2) {
                    area.x + ui->menu->padding,
                    area.y + ui->menu->padding + (font->lineHeight * (float) lineNumber)
            }, font);
}

void drawTextInArea(const char *message, Rectangle area, const FontStyle *font) {
    char m[MAX_MESSAGE_BUFFER];
    char buffer[MAX_LINE_BUFFER];
    strcpy(m, message);
    strcpy(buffer, "");
    char *copy = (char *) m;
    char *word = strtok_r(copy, " ", &copy);
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
        word = strtok_r(copy, " ", &copy);
    }
    if (strcmp(buffer, "") != 0) {
        drawLineInArea(buffer, area, line, font);
    }
    free(word);
}

void updateDialog(Dialog *dialog) {
    double end = getTimeInMS();
    dialog->timeElapsed = dialog->timeElapsed + end - dialog->timeStarted;
    dialog->timeStarted = end;
}

void drawDialog(Dialog *dialog) {
    int amount = (int) ceil(dialog->timeElapsed / 5);
    if (amount == 0) {
        return;
    }
    int len = (int) strlen(dialog->message);
    if (amount > len) {
        amount = len;
    }
    char message[MAX_MESSAGE_BUFFER] = "";
    memcpy(message, &dialog->message[0], amount);
    while (dialog->message[amount - 1] != ' ' && amount < len) {
        strcat(message, "\t");
        amount++;
    }
    drawTextInArea(message, dialog->area, dialog->font);
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

void drawScrollableInMenuWithStyle(TextBox *tb, const FontStyle *fs, const char *text, int menuCursor, int textBoxCursor) {
    float offset = getScrollOffset(fs->lineHeight, menuCursor, tb->area.height);
    float y = tb->area.y
              + line(textBoxCursor != -1 ? textBoxCursor : tb->cursor, fs->lineHeight)
              + ui->menu->padding
              - offset;
    if (y >= tb->area.y) {
        drawText(text, (Vector2) {
                tb->area.x + ui->menu->padding,
                y,
        }, fs);
    }
    if (textBoxCursor == -1) {
        tb->cursor++;
    }
}

void drawInMenuWithStyle(TextBox *tb, const FontStyle *fs, const char *text) {
    drawScrollableInMenuWithStyle(tb, fs, text, 0, -1);
}

void drawScrollableInMenu(TextBox *tb, const char *text, int cursorLine) {
    drawScrollableInMenuWithStyle(tb, tb->fontStyle, text, cursorLine, -1);
}

void drawInMenu(TextBox *tb, const char *text) {
    drawInMenuWithStyle(tb, tb->fontStyle, text);
}
