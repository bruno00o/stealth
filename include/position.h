/**
 * @file position.h
 * @author SEILLIEBERT BRUNO
 * @brief Position functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __POSITION__
#define __POSITION__

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ground.h"

#define SPEED SQUARE_SIZE / 4
#define NB_DIRECTIONS 4

 /**
  * @brief Position structure
  *
  */
typedef struct {
    double x;
    double y;
} Position;

/**
 * @brief Direction enumumeration
 *
 */
typedef enum {
    UP,
    DOWN,
    LEFT,
    RIGHT,
    NONE
} Direction;

/**
 * @brief Returns how many walls the position is in collision with
 *
 * @param position
 * @param ground
 * @return int
 */
int check_collision(Position position, Ground ground);

/**
 * @brief Returns 1 if the position target is in the distance of the position position
 *
 * @param position
 * @param target
 * @param distance
 * @return int
 */
int check_eucledian_distance(Position position, Position target, double distance);

#endif