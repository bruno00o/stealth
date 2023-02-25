/**
 * @file player.h
 * @author SEILLIEBERT BRUNO
 * @brief Player functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __PLAYER__
#define __PLAYER__

#include <stdio.h>
#include <stdlib.h>

#include "ground.h"
#include "position.h"

#define PLAYER_MAX_SPEED 0.9 * SPEED
#define PLAYER_MAX_SPEED_UP 2 * SPEED
#define MANA_GAIN 1

/**
 * @brief Skill enumeration
 *
 */
typedef enum {
    SKILL_NONE,
    INVISIBILITY,
    SPEED_UP
} Skill;

/**
 * @brief Player structure
 *
 */
typedef struct {
    Position position;
    Direction direction;
    double speed;
    int count_relics;
    Skill skill;
    int mana;
    int mana_spent;
} Player;

/**
 * @brief Initialize the player
 *
 * @param player
 * @param ground
 */
void init_player(Player* player, Ground ground);

/**
 * @brief Move the player
 *
 * @param player
 * @param direction
 * @param ground
 */
void move_player(Player* player, Direction direction, Ground ground);

/**
 * @brief Stop the player
 *
 * @param player
 */
void stop_player(Player* player);

/**
 * @brief Returns 1 if the player is on the starting floor
 *
 * @param player
 * @param ground
 * @return int
 */
int is_player_on_starting_floor(Player player, Ground ground);

/**
 * @brief Check if the player is on mana, add mana to the player and change the square to floor
 *
 * @param player
 * @param ground
 */
int player_on_mana(Player* player, Ground* ground);

/**
 * @brief Add the skill given to the player and do actions
 *
 * @param player
 * @param skill
 * @param ground
 */
void add_skill(Player* player, Skill skill, Ground* ground);

/**
 * @brief Remove the skill given to the player
 *
 * @param player
 * @param skill
 * @param ground
 */
void remove_skill(Player* player, Skill skill, Ground* ground);

#endif