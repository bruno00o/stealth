/**
 * @file guardian.h
 * @author SEILLIEBERT BRUNO
 * @brief Guardian functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GUARDIAN__
#define __GUARDIAN__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "position.h"
#include "ground.h"
#include "player.h"

#define NB_GUARDIANS 5
#define GUARDIAN_VISION_RADIUS 4 * SQUARE_SIZE
#define GUARDIAN_VISION_RADIUS_PANIC 6 * SQUARE_SIZE
#define PANIC_DURATION 30

 /**
  * @brief Guardian structure
  *
  */
typedef struct {
    Position position;
    Direction direction;
    double speed;
    double vision_radius;
} Guardian;

/**
 * @brief Guardians list structure
 *
 */
typedef struct {
    Guardian* guardians;
    int count;
    int panic;
    double panic_start;
} Guardians;

/**
 * @brief Initialize guardians
 *
 * @param guardians
 * @param ground
 * @param player
 * @param count
 */
void init_guardians(Guardians* guardians, Ground ground, Player player, int count);

/**
 * @brief Move guardians
 *
 * @param guardians
 * @param ground
 */
void move_guadians(Guardians* guardians, Ground ground);

/**
 * @brief Free guardians
 *
 * @param guardians
 */
void free_guardians(Guardians* guardians);

/**
 * @brief Return 1 if a player is in the vision of a guardian
 *
 * @param guardian
 * @param player
 * @param ground
 * @return int
 */
int is_player_in_guardians_vision(Guardians* guardians, Player player, Ground ground);

/**
 * @brief Return 1 if guardians change their state to panic
 * Return -1 if guardians change their state to not panic
 * Return 0 otherwise
 *
 * @param guardians
 * @param player
 * @param ground
 * @param chrono
 * @return int
 */
int check_panic(Guardians* guardians, Player player, Ground* ground, double chrono);

#endif