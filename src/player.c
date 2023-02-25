/**
 * @file player.c
 * @author SEILLIEBERT BRUNO
 * @brief Player functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/player.h"

 /**
  * @brief Initialize the player
  *
  * @param player
  * @param ground
  */
void init_player(Player* player, Ground ground) {
    int x, y, starting_x, starting_y;
    get_position_of_square(ground, STARTING_FLOOR_X, STARTING_FLOOR_Y, &x, &y);
    starting_x = x + (STARTING_FLOOR_WIDTH * SQUARE_SIZE) / 2;
    starting_y = y + (STARTING_FLOOR_HEIGHT * SQUARE_SIZE) / 2;
    player->position.x = starting_x;
    player->position.y = starting_y;
    player->speed = 0;
    player->direction = NONE;
    player->count_relics = 0;
    player->skill = SKILL_NONE;
    player->mana = 0;
    player->mana_spent = 0;
}

/**
 * @brief Update the player's speed
 *
 * @param player
 * @param direction
 */
static void update_speed(Player* player, Direction direction) {
    if (direction != player->direction) {
        player->speed = 0;
    }
    if (player->speed <= 0) {
        player->speed = 0.1 * SPEED;
    } else if (player->speed < PLAYER_MAX_SPEED) {
        player->speed += 0.03 * SPEED;
    } else {
        if (player->skill == SPEED_UP) {
            player->speed = PLAYER_MAX_SPEED_UP;
        } else {
            player->speed = PLAYER_MAX_SPEED;
        }
    }
}

/**
 * @brief Update the player's direction
 *
 * @param player
 * @param direction
 */
static void update_direction(Player* player, Direction direction) {
    player->direction = direction;
}

/**
 * @brief Update the player's position
 *
 * @param player
 * @param ground
 */
static void update_position(Player* player, Ground ground) {
    Position new_position;
    new_position.x = player->position.x;
    new_position.y = player->position.y;

    switch (player->direction) {
    case UP:
        new_position.y -= player->speed;
        break;
    case DOWN:
        new_position.y += player->speed;
        break;
    case LEFT:
        new_position.x -= player->speed;
        break;
    case RIGHT:
        new_position.x += player->speed;
        break;
    default:
        break;
    }

    if (check_collision(new_position, ground)) {
        player->speed = 0;
    } else {
        player->position = new_position;
    }
}

/**
 * @brief Move the player
 *
 * @param player
 * @param direction
 * @param ground
 */
void move_player(Player* player, Direction direction, Ground ground) {
    update_speed(player, direction);
    update_direction(player, direction);
    update_position(player, ground);
}

/**
 * @brief Stop the player
 *
 * @param player
 */
void stop_player(Player* player) {
    player->speed = 0;
    player->direction = NONE;
}

/**
 * @brief Returns 1 if the player is on the starting floor
 *
 * @param player
 * @param ground
 * @return int
 */
int is_player_on_starting_floor(Player player, Ground ground) {
    if (get_square_type_pixels(ground, player.position.x, player.position.y) == STARTING_FLOOR) {
        return 1;
    }
    return 0;
}

/**
 * @brief Check if the player is on mana, add mana to the player and change the square to floor
 *
 * @param player
 * @param ground
 */
int player_on_mana(Player* player, Ground* ground) {
    int i, j;
    if (player->skill != SKILL_NONE) {
        return 0;
    }
    if (get_square_type_pixels(*ground, player->position.x, player->position.y) == MANA) {
        player->mana += MANA_GAIN;
        get_coordinates_of_square(*ground, player->position.x, player->position.y, &i, &j);
        ground->squares[i][j].square = FLOOR;
        return 1;
    }
    return 0;
}

/**
 * @brief Remove the skill given to the player
 *
 * @param player
 * @param skill
 * @param ground
 */
void remove_skill(Player* player, Skill skill, Ground* ground) {
    if (player->skill == skill) {
        player->skill = SKILL_NONE;
    }
}

/**
 * @brief Return the mana on the ground
 *
 * @param player
 * @param ground
 */
static void return_mana_to_ground(Player player, Ground* ground) {
    int nb_floor, i, j, i_player, j_player, * i_list, * j_list;
    nb_floor = get_nb_square(*ground, FLOOR);
    i_list = (int*)malloc(sizeof(int) * nb_floor);
    j_list = (int*)malloc(sizeof(int) * nb_floor);
    get_coords_square(*ground, i_list, j_list, FLOOR);
    for (i = 0; i < nb_floor / MANA_GAIN - player.mana; i++) {
        j = randint(0, nb_floor - 1);
        get_coordinates_of_square(*ground, player.position.x, player.position.y, &i_player, &j_player);
        if (i_player == i_list[j] && j_player == j_list[j]) {
            continue;
        }
        ground->squares[i_list[j]][j_list[j]].square = MANA;
    }
    free(i_list);
    free(j_list);
}

/**
 * @brief Add the skill given to the player and do actions
 *
 * @param player
 * @param skill
 * @param ground
 */
void add_skill(Player* player, Skill skill, Ground* ground) {
    if (player->mana >= skill) {
        player->mana -= skill;
        return_mana_to_ground(*player, ground);
        player->mana_spent += skill;
        player->skill = skill;
        if (skill == SPEED_UP) {
            player->speed = PLAYER_MAX_SPEED_UP;
        }
    } else {
        remove_skill(player, skill, ground);
    }
}
