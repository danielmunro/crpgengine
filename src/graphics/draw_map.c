#include <math.h>
#include "headers/map.h"
#include "headers/graphics/ui/ui.h"
#include "headers/notification.h"

void drawWarpCollisions(const MapWarps *mw, Image *image) {
    for (int i = 0; i < mw->exitCount; i++) {
        ImageDrawRectangle(
                image,
                mw->exits[i]->area.x,
                mw->exits[i]->area.y,
                mw->exits[i]->area.width,
                mw->exits[i]->area.height,
                WHITE);
    }
    for (int i = 0; i < mw->entranceCount; i++) {
        ImageDrawRectangle(
                image,
                mw->entrances[i]->area.x,
                mw->entrances[i]->area.y,
                mw->entrances[i]->area.width,
                mw->entrances[i]->area.height,
                BLACK);
    }
}

void drawShopCollisions(const Map *m, Image *image) {
    for (int i = 0; i < m->shopTileCount; i++) {
        Rectangle area = rectangleDtoRectangle(m->shopTiles[i]->object->area);
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
        Rectangle a = rectangleDtoRectangle(t->objects[i]->area);
        Rectangle r = {
                (float) (m->context->game->tileSize * x) + a.x,
                (float) (m->context->game->tileSize * y) + a.y,
                a.width,
                a.height,
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
    int sz = tileSize(m->context);
    Vector2 pos = {
            (float) (sz * x),
            (float) (sz * y),
    };
    Rectangle rect = getRectForSpriteByTile(
            m->tileset->sourceTexture.width,
            tileSize(m->context),
            index);
    ImageDraw(
            &layer,
            m->tileset->source,
            rect,
            (Rectangle) {pos.x, pos.y, (float) sz, (float) sz},
            WHITE
    );
    if (m->context->game->showCollisions->objects) {
        drawObjectCollision(m, layer, index, x, y);
    }
}

void renderMapLayer(Map *m, LayerType layer) {
    int tileSize = m->context->game->tileSize;
    int width = m->context->user->resolution.width / tileSize;
    int height = m->context->user->resolution.height / tileSize;
    Image renderedLayer = GenImageColor(width * tileSize, height * tileSize, BLANK);
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
        int y = mobiles[i]->position.y;
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
    int playerY = mob->position.y;
    mobLayer[playerY][mobsByYPosition[playerY]] = mob;

    /**
     * Now go through the layer and draw mobs in order.
     */
    const UIConfig *ui = m->context->ui;
    for (int y = 0; y < MAX_LAYERS; y++) {
        for (int n = 0; n < MAX_MOBILES; n++) {
            if (mobLayer[y][n] == NULL) {
                break;
            }
            drawAnimation(
                    getActiveMobAnimation(mobLayer[y][n]),
                    (Vector2) {
                            (((float) mobLayer[y][n]->position.x * m->context->ui->screen->scale) + offset.x),
                            (floorf(((float) mobLayer[y][n]->position.y * m->context->ui->screen->scale) + offset.y) -
                                    (float) tileSize(m->context)),
                    }
            );
            if (m->context->game->showCollisions->objects) {
                Rectangle rect = getMobileRectangle(mobLayer[y][n]);
                DrawRectangle(
                        (int) ((rect.x * ui->screen->scale) + offset.x),
                        (int) ((rect.y * ui->screen->scale) + offset.y),
                        (int) (rect.width * ui->screen->scale),
                        (int) (rect.height * ui->screen->scale),
                        BLUE);
            }
        }
    }

    if (m->context->game->showCollisions->warps) {
        for (int i = 0; i < m->warps->arriveAtCount; i++) {
            Rectangle r = rectangleDtoRectangle(m->warps->arriveAt[i]->rect);
            DrawRectangle(
                    (int) ((r.x * ui->screen->scale) + offset.x),
                    (int) ((r.y * ui->screen->scale) + offset.y),
                    (int) (r.width * ui->screen->scale),
                    (int) (r.height * ui->screen->scale),
                    YELLOW);
        }
    }

    if (m->context->game->showCollisions->player) {
        Rectangle rect = getMobileRectangle(getPartyLeader(p));
        DrawRectangle(
                (int) ((rect.x * ui->screen->scale) + offset.x),
                (int) ((rect.y * ui->screen->scale) + offset.y),
                (int) (rect.width * ui->screen->scale),
                (int) (rect.height * ui->screen->scale),
                GREEN);
    }
}

void drawChests(const Map *m, const Player *p, Vector2 offset) {
    const UIConfig *ui = m->context->ui;
    int tileSize = m->context->game->tileSize;
    for (int i = 0; i < m->chestCount; i++) {
        Rectangle dest = {
                ((float) m->chests[i]->area.x * ui->screen->scale) + offset.x,
                ((float) m->chests[i]->area.y * ui->screen->scale) + offset.y,
                (float) tileSize * ui->screen->scale,
                (float) tileSize * ui->screen->scale };
        Vector2 origin = { 0.0f, 0.0f };
        int index;
        if (isChestOpened(p, m->sceneId, m->chests[i]->id)) {
            index = m->openedChest->id;
        } else {
            index = m->closedChest->id;
        }
        Rectangle src = getRectForSpriteByIndex(
                m->tileset->sourceTexture.width,
                tileSize,
                tileSize,
                index);
        DrawTexturePro(
                m->tileset->sourceTexture,
                src,
                dest,
                origin,
                0.0f,
                WHITE);
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
    ClearBackground(BLACK);
    const UIConfig *ui = m->context->ui;
    Vector2 offset = {
            ((float) m->context->user->resolution.width / 2) - ((float) mob->position.x * ui->screen->scale),
            ((float) m->context->user->resolution.height / 2) - ((float) mob->position.y * ui->screen->scale),
    };
    DrawTextureEx(m->renderedLayers[BACKGROUND], offset, 0, ui->screen->scale, WHITE);
    DrawTextureEx(m->renderedLayers[MIDGROUND], offset, 0, ui->screen->scale, WHITE);
    drawChests(m, p, offset);
    drawExplorationMobiles(m, p, offset);
    DrawTextureEx(m->renderedLayers[FOREGROUND], offset, 0, ui->screen->scale, WHITE);
    drawNotifications(nm, font);
    drawExplorationControls(p, c, font, m->context->ui);
    if (m->context->game->showFPS) {
        DrawFPS(FPS_X, FPS_Y);
    }
}
