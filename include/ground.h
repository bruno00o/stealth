/**
 * @file ground.h
 * @author SEILLIEBERT BRUNO
 * @brief Ground functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __GROUND__
#define __GROUND__

#include <stdio.h>
#include <stdlib.h>

#define GROUND_WIDTH 60
#define GROUND_HEIGHT 45

#define SQUARE_SIZE 20

#define STARTING_FLOOR_WIDTH 2
#define STARTING_FLOOR_HEIGHT 2
#define STARTING_FLOOR_X 1
#define STARTING_FLOOR_Y 1

 /**
  * @brief Enum containing the different types of squares
  *
  */
typedef enum {
    WALL,
    RELIC,
    RELIC_TAKEN,
    RELIC_TAKEN_AND_SEEN,
    FLOOR,
    STARTING_FLOOR,
    MANA
} SquareType;

/**
 * @brief Struct of a square of the ground with its type and its coordinates
 *
 */
typedef struct {
    SquareType square;
    int i;
    int j;
} GroundSquare;

/**
 * @brief Struct of the ground with its squares and its width and height in pixels
 *
 */
typedef struct {
    GroundSquare squares[GROUND_WIDTH][GROUND_HEIGHT];
    int pixel_width;
    int pixel_height;
} Ground;

/**
 * @brief Initialize the ground
 *
 * @param ground
 */
void init_ground(Ground* ground);

/**
 * @brief Get the position of a square on the ground in pixels based on its coordinates in the ground
 * (x, y) is the top left corner of the square
 *
 * @param ground
 * @param i
 * @param j
 * @param x
 * @param y
 */
void get_position_of_square(Ground ground, int i, int j, int* x, int* y);

/**
 * @brief Get the coordinates of square on the ground based on its position in pixels
 *
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 */
void get_coordinates_of_square(Ground ground, int x, int y, int* i, int* j);

/**
 * @brief Generate a random number between min and max
 *
 * @param min
 * @param max
 * @return int
 */
int randint(int min, int max);

/**
 * @brief Return the type of a square on the ground based on its position in pixels
 *
 * @param ground
 * @param x
 * @param y
 * @return SquareType
 */
SquareType get_square_type_pixels(Ground ground, int x, int y);

/**
 * @brief Return 1 if a wall is placed around (i, j) in the ground at a distance of "distance"
 * Return 0 otherwise
 *
 * @param ground
 * @param i
 * @param j
 * @param distance
 * @return int
 */
int is_wall_placed_around(Ground ground, int i, int j, int distance);

/**
 * @brief Return the number of squares of a given type on the ground
 *
 * @param ground
 * @param square_type
 * @return int
 */
int get_nb_square(Ground ground, SquareType square_type);

/**
 * @brief Get the coordinates of all the squares of a given type on the ground
 *
 * @param ground
 * @param i_list
 * @param j_list
 * @param square_type
 */
void get_coords_square(Ground ground, int i_list[], int j_list[], SquareType square_type);

#endif