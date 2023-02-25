/**
 * @file relic.h
 * @author SEILLIEBERT BRUNO
 * @brief Relic functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __RELIC__
#define __RELIC__

#include <stdio.h>
#include <stdlib.h>

#include "ground.h"
#include "player.h"

#define NB_RELICS 3

 /**
  * @brief Relic structure
  *
  */
typedef struct {
    int i;
    int j;
    int is_taken;
} Relic;

/**
 * @brief Relics structure
 *
 */
typedef struct {
    Relic* relics;
    int count;
} Relics;

/**
 * @brief Initialize the relic structure
 *
 * @param relic
 * @param ground
 * @param player
 */
void init_relics(Relics* relics, Ground* ground, Player player, int count);

/**
 * @brief Free the relics structure
 *
 * @param relics
 */
void free_relics(Relics* relics);

/**
 * @brief Check if the player is on a relic and if it is, take it
 *
 * @param relics
 * @param ground
 * @param player
 */
int relic_found(Relics* relics, Ground* ground, Player* player);

#endif