/**
 * @file position.c
 * @author SEILLIEBERT BRUNO
 * @brief Position functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/position.h"

 /**
  * @brief Returns 1 if the position is in collision with a wall on the top
  *
  * @param position
  * @param ground
  * @param x
  * @param y
  * @param i
  * @param j
  * @return int
  */
static int check_top_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i][j - 1];
    if (square.square == WALL) {
        if (position.y <= y + 0.5 * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns 1 if the position is in collision with a wall on the right
 *
 * @param position
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 * @return int
 */
static int check_right_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i + 1][j];
    if (square.square == WALL) {
        if (position.x >= x + 0.5 * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns 1 if the position is in collision with a wall on the bottom
 *
 * @param position
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 * @return int
 */
static int check_bottom_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i][j + 1];
    if (square.square == WALL) {
        if (position.y >= y + 0.5 * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns 1 if the position is in collision with a wall on the left
 *
 * @param position
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 * @return int
 */
static int check_left_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i - 1][j];
    if (square.square == WALL) {
        if (position.x <= x + 0.5 * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns 1 if the position is in collision with a wall on the top left
 *
 * @param position
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 * @return int
 */
static int check_top_left_square_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i - 1][j - 1];
    if (square.square == WALL && position.x < x + 0.5 * SQUARE_SIZE) {
        if (position.y <= y + sqrt(0.25 - pow((position.x - x) / SQUARE_SIZE, 2)) * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns 1 if the position is in collision with a wall on the top right
 *
 * @param position
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 * @return int
 */
static int check_top_right_square_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i + 1][j - 1];
    if (square.square == WALL && position.x >= x + 0.5 * SQUARE_SIZE) {
        if (position.y <= y + sqrt(0.25 - pow((x + SQUARE_SIZE - position.x) / SQUARE_SIZE, 2)) * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns 1 if the position is in collision with a wall on the bottom left
 *
 * @param position
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 * @return int
 */
static int check_bottom_left_square_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i - 1][j + 1];
    if (square.square == WALL && position.x < x + 0.5 * SQUARE_SIZE) {
        if (position.y >= y + SQUARE_SIZE - sqrt(0.25 - pow((position.x - x) / SQUARE_SIZE, 2)) * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns 1 if the position is in collision with a wall on the bottom right
 *
 * @param position
 * @param ground
 * @param x
 * @param y
 * @param i
 * @param j
 * @return int
 */
static int check_bottom_right_square_collision(Position position, Ground ground, int x, int y, int i, int j) {
    GroundSquare square = ground.squares[i + 1][j + 1];
    if (square.square == WALL && position.x >= x + 0.5 * SQUARE_SIZE) {
        if (position.y >= y + SQUARE_SIZE - sqrt(0.25 - pow((x + SQUARE_SIZE - position.x) / SQUARE_SIZE, 2)) * SQUARE_SIZE) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Returns how many walls the position is in collision with
 *
 * @param position
 * @param ground
 * @return int
 */
int check_collision(Position position, Ground ground) {
    int i, j, x, y, collision;
    get_coordinates_of_square(ground, position.x, position.y, &i, &j);
    get_position_of_square(ground, i, j, &x, &y);

    collision = check_top_collision(position, ground, x, y, i, j);
    collision += check_right_collision(position, ground, x, y, i, j);
    collision += check_bottom_collision(position, ground, x, y, i, j);
    collision += check_left_collision(position, ground, x, y, i, j);
    collision += check_top_left_square_collision(position, ground, x, y, i, j);
    collision += check_top_right_square_collision(position, ground, x, y, i, j);
    collision += check_bottom_left_square_collision(position, ground, x, y, i, j);
    collision += check_bottom_right_square_collision(position, ground, x, y, i, j);

    return collision;
}

/**
 * @brief Returns 1 if the position target is in the distance of the position position
 *
 * @param position
 * @param target
 * @param distance
 * @return int
 */
int check_eucledian_distance(Position position, Position target, double distance) {
    if (sqrt(pow(position.x - target.x, 2) + pow(position.y - target.y, 2)) <= distance) {
        return 1;
    }
    return 0;
}
