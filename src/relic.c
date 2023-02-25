/**
 * @file relic.c
 * @author SEILLIEBERT BRUNO
 * @brief Relic functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/relic.h"

 /**
  * @brief Initialize the relic structure
  *
  * @param relic
  * @param ground
  * @param player
  */
static void init_relic(Relic* relic, Ground* ground, Player player) {
    int x, y, i, j;
    Position position;
    do {
        x = rand() % ground->pixel_height;
        y = rand() % ground->pixel_width;
        position.x = x;
        position.y = y;
        get_coordinates_of_square(*ground, x, y, &i, &j);
    } while (
        (i < 0 || i >= GROUND_WIDTH) || (j < 0 || j >= GROUND_HEIGHT) ||
        get_square_type_pixels(*ground, x, y) == WALL ||
        check_eucledian_distance(player.position, position, 20 * SQUARE_SIZE) == 1 ||
        is_wall_placed_around(*ground, i, j, 2) == 1
        );
    relic->i = i;
    relic->j = j;
    ground->squares[i][j].square = RELIC;
    relic->is_taken = 0;
}

/**
 * @brief Initialize the relics structure
 *
 * @param relics
 * @param ground
 * @param player
 * @param count
 */
void init_relics(Relics* relics, Ground* ground, Player player, int count) {
    int i;
    relics->count = count;
    relics->relics = malloc(count * sizeof(Relic));
    if (relics->relics == NULL) {
        fprintf(stderr, "Error: malloc failed in init_relics\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < count; i++) {
        init_relic(&relics->relics[i], ground, player);
    }
}

/**
 * @brief Free the relics structure
 *
 * @param relics
 */
void free_relics(Relics* relics) {
    free(relics->relics);
}

/**
 * @brief Check if the player is on a relic and if it is, take it
 *
 * @param relics
 * @param ground
 * @param player
 */
int relic_found(Relics* relics, Ground* ground, Player* player) {
    int k, x, y, i, j;
    x = player->position.x;
    y = player->position.y;
    get_coordinates_of_square(*ground, x, y, &i, &j);
    for (k = 0; k < relics->count; k++) {
        if (!relics->relics[k].is_taken && relics->relics[k].i == i && relics->relics[k].j == j) {
            relics->relics[k].is_taken = 1;
            player->count_relics++;
            ground->squares[relics->relics[k].i][relics->relics[k].j].square = RELIC_TAKEN;
            return 1;
        }
    }
    return 0;
}