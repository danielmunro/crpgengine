#include <math.h>
#include "headers/map.h"
#include "headers/graphics/ui/ui.h"
#include "headers/notification.h"

void drawWarpCollisions(const MapWarps *mw, Image *image) {
    for (int i = 0; i < mw->exitCount; i++) {
        ImageDrawRectangle(
                image,
                (int) (mw->exits[i]->area.x),
                (int) (mw->exits[i]->area.y),
                (int) (mw->exits[i]->area.width),
                (int) (mw->exits[i]->area.height),
                WHITE);
    }
    for (int i = 0; i < mw->entranceCount; i++) {
        ImageDrawRectangle(
                image,
                (int) (mw->entrances[i]->area.x),
                (int) (mw->entrances[i]->area.y),
                (int) (mw->entrances[i]->area.width),
                (int) (mw->entrances[i]->area.height),
                BLACK);
    }
}

void drawShopCollisions(const Map *m, Image *image) {
    for (int i = 0; i < m->shopTileCount; i++) {
        Rectangle area = m->shopTiles[i]->object->area;
        ImageDrawRectangle(
                image,
                (int) area.x,
                (int) area.y,
                (int) area.width,
                (int) area.height,
                GRAY);
    }
}

void drawObjectCollision(const Map *m, Image layer, int tileNumber, int x, int y) {
    const Tile *t = getTile(m, tileNumber);
    if (t == NULL) {
        return;
    }
    for (int i = 0; i < t->objectCount; i++) {
        if (t->objects[i] == NULL) {
            return;
        }
        Rectangle r = {
                (float) (m->tileset->size.x * x) + t->objects[i]->area.x,
                (float) (m->tileset->size.y * y) + t->objects[i]->area.y,
                t->objects[i]->area.width,
                t->objects[i]->area.height,
        };
        ImageDrawRectangle(
                &layer,
                (int) r.x,
                (int) r.y,
                (int) r.width,
                (int) r.height,
                PINK
        );
    }
}

void drawTile(const Map *m, Image layer, int index, int x, int y) {
    if (index <= 0) {
        return;
    }
    Vector2D sz = m->config->tileSize;
    Vector2 pos = {
            (float) (sz.x * x),
            (float) (sz.y * y),
    };
    Rectangle rect = getRectForTile(m, index);
    ImageDraw(
            &layer,
            m->tileset->source,
            rect,
            (Rectangle) {pos.x, pos.y, (float) sz.x, (float) sz.y},
            WHITE
    );
    if (m->context->game->showCollisions->objects) {
        drawObjectCollision(m, layer, index, x, y);
    }
}

void renderMapLayer(Map *m, LayerType layer) {
    Vector2D sz = m->tileset->size;
    int width = m->context->user->resolution.width / sz.x;
    int height = m->context->user->resolution.height / sz.y;
    Image renderedLayer = GenImageColor(width * sz.x, height * sz.y, BLANK);
    for (int y = -1; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (x >= m->layers[layer]->width || y >= m->layers[layer]->height) {
                continue;
            }
            drawTile(
                    m,
                    renderedLayer,
                    m->layers[layer]->data[y][x],
                    x,
                    y
            );
        }
    }
    if (m->context->game->showCollisions->warps) {
        drawWarpCollisions(m->warps, &renderedLayer);
    }
    if (m->context->game->showCollisions->objects) {
        drawShopCollisions(m, &renderedLayer);
    }
    m->renderedLayers[layer] = LoadTextureFromImage(renderedLayer);
    UnloadImage(renderedLayer);
}

void renderExplorationLayers(Map *m) {
    ClearBackground(BLACK);
    renderMapLayer(m, BACKGROUND);
    renderMapLayer(m, MIDGROUND);
    renderMapLayer(m, FOREGROUND);
    addDebug("map successfully rendered");
}

void prepareMapLayers(Map *m) {
    m->tileset->source = LoadImage(m->tileset->sourcePath);
    m->tileset->sourceTexture = LoadTextureFromImage(m->tileset->source);
    UnloadImage(m->tileset->source);
    renderExplorationLayers(m);
}

void unloadMapLayers(const Map *m) {
    UnloadTexture(m->renderedLayers[BACKGROUND]);
    UnloadTexture(m->renderedLayers[MIDGROUND]);
    UnloadTexture(m->renderedLayers[FOREGROUND]);
    UnloadTexture(m->tileset->sourceTexture);
}

void createMobileLayer(
        Mobile *mobiles[MAX_MOBILES],
        Mobile *mobLayer[MAX_LAYERS][MAX_MOBILES],
        int mobileCount,
        int mobsByYPosition[MAX_LAYERS]) {
    for (int y = 0; y < MAX_LAYERS; y++) {
        for (int i = 0; i < MAX_MOBILES; i++) {
            mobLayer[y][i] = NULL;
        }
    }
    for (int i = 0; i < MAX_LAYERS; i++) {
        mobsByYPosition[i] = 0;
    }
    for (int i = 0; i < mobileCount; i++) {
        int y = (int) roundf(mobiles[i]->position.y);
        mobLayer[y][mobsByYPosition[y]] = mobiles[i];
        mobsByYPosition[y]++;
    }
}

void drawExplorationMobiles(Map *m, const Player *p, Vector2 offset) {
    /**
     * Start by putting mobs on a layer. This is necessary for drawing them in
     * the right order.
     */
    Mobile *mobLayer[MAX_LAYERS][MAX_MOBILES];
    int mobsByYPosition[MAX_LAYERS];
    createMobileLayer(m->mobiles, mobLayer, m->mobileCount, mobsByYPosition);

    /**
     * The player goes on the layer too.
     */
    Mobile *mob = getPartyLeader(p);
    int playerY = (int) roundf(mob->position.y);
    mobLayer[playerY][mobsByYPosition[playerY]] = mob;

    /**
     * Now go through the layer and draw mobs in order.
     */
    for (int y = 0; y < MAX_LAYERS; y++) {
        for (int n = 0; n < MAX_MOBILES; n++) {
            if (mobLayer[y][n] == NULL) {
                break;
            }
            drawAnimation(
                    getMobAnimation(mobLayer[y][n]),
                    (Vector2) {
                            ((mobLayer[y][n]->position.x * m->context->ui->screen->scale) + offset.x),
                            (floorf((mobLayer[y][n]->position.y * m->context->ui->screen->scale) + offset.y)),
                    }
            );
        }
    }

    if (m->context->game->showCollisions->player) {
        const UIConfig *ui = m->context->ui;
        Rectangle rect = getMobileRectangle(getPartyLeader(p));
        DrawRectangle(
                (int) ((rect.x * ui->screen->scale) + offset.x),
                (int) ((rect.y * ui->screen->scale) + offset.y),
                (int) (rect.width * ui->screen->scale),
                (int) (rect.height * ui->screen->scale),
                GREEN);
    }
}

void drawOpenedChests(const Map *m, const Player *p, Vector2 offset) {
    const UIConfig *ui = m->context->ui;
    for (int i = 0; i < m->chestCount; i++) {
        if (isChestOpened(p, m->sceneId, m->chests[i]->id)) {
            Rectangle dest = {
                    (m->chests[i]->area.x * ui->screen->scale) + offset.x,
                    (m->chests[i]->area.y * ui->screen->scale) + offset.y,
                    (float) m->tileset->size.x * ui->screen->scale,
                    (float) m->tileset->size.y * ui->screen->scale };
            Vector2 origin = { 0.0f, 0.0f };
            DrawTexturePro(
                    m->tileset->sourceTexture,
                    getRectForTile(m, m->openedChest->id + 1),
                    dest,
                    origin,
                    0.0f,
                    WHITE);
        }
    }
}

void drawExplorationControls(
        Player *player,
        ControlBlock *cb[MAX_ACTIVE_CONTROLS],
        const FontStyle *font,
        const UIConfig *ui) {
    if (player->dialog) {
        drawMenuRect(ui->menu, ui->textAreas->bottom);
        drawDialog(player->dialog, ui->menu->padding);
        return;
    }
    for (int i = 0; i < MAX_ACTIVE_CONTROLS; i++) {
        if (cb[i] == NULL) {
            continue;
        }
        int p = cb[i]->progress;
        if (cb[i]->thenCount > p
            && cb[i]->then[p]->outcome == SPEAK
            && isSpeakingTo(player, cb[i]->then[p]->target)) {
            Rectangle area = ui->textAreas->bottom;
            if (player->dialog == NULL) {
                player->dialog = createDialog(
                        cb[i]->then[p]->message,
                        area,
                        font);
            }
            drawMenuRect(ui->menu, area);
            drawDialog(player->dialog, ui->menu->padding);
        }
    }
}

void drawMapView(Map *m, Player *p, NotificationManager *nm, ControlBlock *c[64], FontStyle *font) {
    const Mobile *mob = getPartyLeader(p);
    BeginDrawing();
    ClearBackground(BLACK);
    const UIConfig *ui = m->context->ui;
    Vector2 offset = {
            ((float) m->context->user->resolution.width / 2) - (mob->position.x * ui->screen->scale),
            ((float) m->context->user->resolution.height / 2) - (mob->position.y * ui->screen->scale),
    };
    DrawTextureEx(m->renderedLayers[BACKGROUND], offset, 0, ui->screen->scale, WHITE);
    DrawTextureEx(m->renderedLayers[MIDGROUND], offset, 0, ui->screen->scale, WHITE);
    drawOpenedChests(m, p, offset);
    drawExplorationMobiles(m, p, offset);
    DrawTextureEx(m->renderedLayers[FOREGROUND], offset, 0, ui->screen->scale, WHITE);
    drawNotifications(nm, font);
    drawExplorationControls(p, c, font, m->context->ui);
    if (m->context->game->showFPS) {
        DrawFPS(FPS_X, FPS_Y);
    }
    EndDrawing();
}
