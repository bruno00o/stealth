/**
 * @file ground.c
 * @author SEILLIEBERT BRUNO
 * @brief Ground functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/ground.h"

 /**
  * @brief Place the walls on the border of the ground
  *
  * @param ground
  */
static void place_walls_border(Ground* ground) {
    int i;
    for (i = 0; i < GROUND_WIDTH; i++) {
        ground->squares[i][0].square = WALL;
        ground->squares[i][GROUND_HEIGHT - 1].square = WALL;
    }
    for (i = 0; i < GROUND_HEIGHT; i++) {
        ground->squares[0][i].square = WALL;
        ground->squares[GROUND_WIDTH - 1][i].square = WALL;
    }
}

/**
 * @brief Place a wall between start and end coordinates on the ground
 * It handles the case where the wall is vertical or horizontal
 *
 * @param ground
 * @param start
 * @param end
 * @param is_vertical
 * @param start_coord
 */
static void place_wall(Ground* ground, int start, int end, int is_vertical, int start_coord) {
    int i, opening_side;
    opening_side = rand() % 2;
    start = opening_side == 0 ? start + 3 : start;
    end = opening_side == 0 ? end : end - 3;
    if (is_vertical) {
        for (i = start; i <= end; i++) {
            ground->squares[start_coord][i].square = WALL;
        }
    } else {
        for (i = start; i <= end; i++) {
            ground->squares[i][start_coord].square = WALL;
        }
    }
}

/**
 * @brief Generate a random number between min and max
 *
 * @param min
 * @param max
 * @return int
 */
int randint(int min, int max) {
    return rand() % (max - min + 1) + min;
}


/**
 * @brief Place the walls on the ground
 *
 * @param ground
 * @param start_x
 * @param start_y
 * @param end_x
 * @param end_y
 * @param minside
 */
static void place_walls(Ground* ground, int start_x, int start_y, int end_x, int end_y, int minside) {
    int x1, y1;
    if (end_x - start_x < 2 * minside + 1 || end_y - start_y < 2 * minside + 1) {
        return;
    }
    if (end_x - start_x > end_y - start_y) {
        if (end_x - start_x < 4 * minside && rand() % 2 == 0) {
            return;
        }
        x1 = randint(start_x + minside, end_x - minside);
        place_wall(ground, start_y, end_y, 1, x1);

        place_walls(ground, start_x, start_y, x1 - 1, end_y, minside);
        place_walls(ground, x1 + 1, start_y, end_x, end_y, minside);
    } else {
        if (end_y - start_y < 4 * minside && rand() % 2 == 0) {
            return;
        }
        y1 = randint(start_y + minside, end_y - minside);
        place_wall(ground, start_x, end_x, 0, y1);

        place_walls(ground, start_x, start_y, end_x, y1 - 1, minside);
        place_walls(ground, start_x, y1 + 1, end_x, end_y, minside);
    }
}

/**
 * @brief Place the starting floor on the ground
 *
 * @param ground
 */
static void place_starting_floor(Ground* ground) {
    int i, j;
    for (i = STARTING_FLOOR_X; i < STARTING_FLOOR_X + STARTING_FLOOR_WIDTH; i++) {
        for (j = STARTING_FLOOR_Y; j < STARTING_FLOOR_Y + STARTING_FLOOR_HEIGHT; j++) {
            ground->squares[i][j].square = STARTING_FLOOR;
        }
    }
}

/**
 * @brief Initialize the ground
 *
 * @param ground
 */
void init_ground(Ground* ground) {
    int i, j;
    for (i = 0; i < GROUND_WIDTH; i++) {
        for (j = 0; j < GROUND_HEIGHT; j++) {
            ground->squares[i][j].square = MANA;
            ground->squares[i][j].i = i;
            ground->squares[i][j].j = j;
        }
    }

    place_walls_border(ground);
    place_walls(ground, 1, 1, GROUND_WIDTH - 2, GROUND_HEIGHT - 2, 9);
    place_starting_floor(ground);

    ground->pixel_width = GROUND_WIDTH * SQUARE_SIZE;
    ground->pixel_height = GROUND_HEIGHT * SQUARE_SIZE;
}

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
void get_position_of_square(Ground ground, int i, int j, int* x, int* y) {
    *x = ground.squares[i][j].i * SQUARE_SIZE;
    *y = ground.squares[i][j].j * SQUARE_SIZE;
}

/**
 * @brief Get the coordinates of square on the ground based on its position in pixels
 *
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 */
void get_coordinates_of_square(Ground ground, int x, int y, int* i, int* j) {
    *i = x / SQUARE_SIZE;
    *j = y / SQUARE_SIZE;
}

/**
 * @brief Return the type of a square on the ground based on its coordinates in the ground
 *
 * @param ground
 * @param i
 * @param j
 * @return SquareType
 */
static SquareType get_square_type(Ground ground, int i, int j) {
    return ground.squares[i][j].square;
}

/**
 * @brief Return the type of a square on the ground based on its position in pixels
 *
 * @param ground
 * @param x
 * @param y
 * @return SquareType
 */
SquareType get_square_type_pixels(Ground ground, int x, int y) {
    int i, j;
    get_coordinates_of_square(ground, x, y, &i, &j);
    return get_square_type(ground, i, j);
}

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
int is_wall_placed_around(Ground ground, int i, int j, int distance) {
    int k, l;
    for (k = i - distance; k <= i + distance; k++) {
        for (l = j - distance; l <= j + distance; l++) {
            if (k >= 0 && k < GROUND_WIDTH && l >= 0 && l < GROUND_HEIGHT) {
                if (ground.squares[k][l].square == WALL) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/**
 * @brief Return the number of squares of a given type on the ground
 *
 * @param ground
 * @param square_type
 * @return int
 */
int get_nb_square(Ground ground, SquareType square_type) {
    int i, j, nb_square = 0;
    for (i = 0; i < GROUND_WIDTH; i++) {
        for (j = 0; j < GROUND_HEIGHT; j++) {
            if (ground.squares[i][j].square == square_type) {
                nb_square++;
            }
        }
    }
    return nb_square;
}

/**
 * @brief Get the coordinates of all the squares of a given type on the ground
 *
 * @param ground
 * @param i_list
 * @param j_list
 * @param square_type
 */
void get_coords_square(Ground ground, int i_list[], int j_list[], SquareType square_type) {
    int i, j, k = 0;
    for (i = 0; i < GROUND_WIDTH; i++) {
        for (j = 0; j < GROUND_HEIGHT; j++) {
            if (ground.squares[i][j].square == square_type) {
                i_list[k] = i;
                j_list[k] = j;
                k++;
            }
        }
    }
}
