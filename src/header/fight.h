//
// Created by Dan Munro on 9/5/23.
//

#ifndef CJRPGENGINE_FIGHT_H
#define CJRPGENGINE_FIGHT_H

#define MAX_BEASTIARY_SIZE 1024
#define MAX_BEASTS 255
#define MAX_BEASTS_IN_FIGHT 9
#define MAX_BEAST_NAMES_IN_FIGHT 6

#define MAX_CURSORS 64
#define MAX_ACTION_GAUGE 300
#define MAX_ACTION_GAUGE_RAISE 10

#define MAX_ACTIONS 256

typedef enum {
    ATTACK,
    CAST,
    ITEM,
    NONE,
} ActionType;

typedef enum {
    STEP_NONE,
    ATTACK_QUEUE,
    ATTACK_STEP_OUT,
    ATTACK_ACTION,
    ATTACK_RETURN,
} ActionStep;

#define STEP_OUT_TIMEOUT_MS 300
#define ACTION_TIMEOUT_MS 100
#define RETURN_TIMEOUT_MS 300

#endif //CJRPGENGINE_FIGHT_H
