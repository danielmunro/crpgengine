void drawFightBackground(Encounters *encounters) {
    float scale = (float) ui->screen->width / (float) encounters->background.width;
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
        (Vector2) { x, beast->position.y },
        0, ui->screen->scale, WHITE);
    }
}

void drawFightPlayer(Player *player) {
    for (int i = 0; i < MAX_PARTY_SIZE; i++) {
        if (player->party[i] != NULL) {
            Spritesheet *sprite = player->party[i]->animations[0]->spriteSheet;
            Vector2 position = getFightPlayerPosition(i, sprite->frameHeight);
            if (player->party[i]->step == ATTACK_STEP_OUT) {
                position.x -= (float) sprite->frameWidth;
            }
            if (player->party[i]->hitAnimationTimer > 0) {
                position.x += getHitAnimationX(player->party[i]->hitAnimationTimer);
            }
            drawAnimation(
                    findAnimation(getPartyLeader(player)->animations, LEFT),
                    position);
        }
    }
}

void drawFightMenu(FightManager *fm) {
    drawAllMenus(fm->ui->menuContext, fm->menus);
}

void drawFightView(Encounters *encounters, FightManager *fights) {
    BeginDrawing();
    ClearBackground(BLACK);
    drawFightBackground(encounters);
    drawFightBeasts(fights->fight);
    drawFightPlayer(fights->fight->player);
    drawFightMenu(fights);
    if (runtimeArgs->showFPS) {
        DrawFPS(FPS_X, FPS_Y);
    }
    EndDrawing();
}
