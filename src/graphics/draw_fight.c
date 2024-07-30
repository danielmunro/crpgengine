#include <raylib.h>
#include "headers/notification.h"

void drawFightBackground(Encounters *encounters, Resolution r) {
    float scale = (float) r.width / (float) encounters->background.width;
    DrawTextureEx(encounters->background, (Vector2) {0, 0}, 0, scale, WHITE);
}

float getHitAnimationX(float timer) {
    if (timer > 0) {
        int mod = (int) timer % 100;
        if (mod > 50) {
            return 3;
        } else {
            return -3;
        }
    }
    return 0;
}

void drawFightBeasts(Fight *fight) {
    for (int i = 0; i < fight->beastCount; i++) {
        Beast *beast = fight->beasts[i];
        float x = beast->position.x;
        if (beast->step == ATTACK_STEP_OUT) {
            x += (float) beast->image.width;
        }
        if (beast->hitAnimationTimer > 0) {
            x += getHitAnimationX(beast->hitAnimationTimer);
        }
        DrawTextureEx(beast->image,
                      (Vector2) {x, beast->position.y},
                      0, fight->context->ui->screen->scale, WHITE);
    }
}

void drawFightPlayer(Player *player, Resolution r, int targetFPS) {
    for (int i = 0; i < player->partyCount; i++) {
        Mobile *mob = player->party[i];
        Spritesheet *sprite = mob->animations[0]->spriteSheet;
        Vector2 position = getFightPlayerPosition(i, sprite->frameHeight, r);
        if (mob->step == ATTACK_STEP_OUT) {
            position.x -= (float) sprite->frameWidth;
        }
        if (mob->hitAnimationTimer > 0) {
            position.x += getHitAnimationX(mob->hitAnimationTimer);
        }
        AnimationType animationType = mob->isFleeing ? ANIMATION_RIGHT : ANIMATION_LEFT;
        Animation *animation = findAnimation(mob->animations, animationType);
        if (mob->isFleeing) {
            incrementAnimationFrame(animation, (float) targetFPS);
        }
        drawAnimation(animation, position);
    }
}

void drawFightMenu(FightManager *fm) {
    drawAllMenus(fm->ui->menuContext, fm->menus);
}

void drawFightView(
        Encounters *encounters,
        FightManager *fights,
        Context *c) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawFightBackground(encounters, c->user->resolution);
    drawFightBeasts(fights->fight);
    drawFightPlayer(
            fights->fight->player,
            c->user->resolution,
            c->ui->screen->targetFrameRate);
    drawFightMenu(fights);
    drawNotifications(fights->notifications, fights->ui->fonts->default_);
    if (c->game->showFPS) {
        DrawFPS(FPS_X, FPS_Y);
    }
    EndDrawing();
}
