/**
 * @file game.h
 * @author SEILLIEBERT BRUNO
 * @brief Game functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GAME__
#define __GAME__

#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <MLV/MLV_all.h>

#include "ground.h"
#include "player.h"
#include "guardian.h"
#include "relic.h"
#include "leaderboard.h"
#include "display.h"
#include "keys.h"

 /**
  * @brief Game state enumeration
  *
  */
typedef enum {
    START,
    GAME,
    WIN,
    LOOSE,
    BREAK
} GameState;

/**
 * @brief Game structure
 *
 */
typedef struct {
    Ground ground;
    Player player;
    Guardians guardians;
    Relics relics;
    Leaderboard leaderboard;
    double chrono;
    GameState state;
} Game;

/**
 * @brief Initialize the game
 *
 * @param game
 */
void init_game(Game* game);

/**
 * @brief Play the game
 *
 * @param game
 */
void play_game(Game* game);

/**
 * @brief Free the game
 *
 * @param game
 */
void free_game(Game* game);

#endif