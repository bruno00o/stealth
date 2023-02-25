/**
 * @file guardian.c
 * @author SEILLIEBERT BRUNO
 * @brief Guardian functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/guardian.h"

 /**
 * @brief Reset the speed and direction of a guardian
 *
 * @param guardian
 */
static void reset_speed_direction(Guardian* guardian) {
    guardian->speed = randint(3, 8) / 10.0 * SPEED;
    guardian->direction = rand() % NB_DIRECTIONS;
}

/**
 * @brief Initialize a guardian
 *
 * @param guardian
 * @param ground
 * @param player
 */
static void init_guardian(Guardian* guardian, Ground ground, Player player) {
    int x, y, i, j;
    Position position;
    do
    {
        x = rand() % ground.pixel_width;
        y = rand() % ground.pixel_height;
        position.x = x;
        position.y = y;
        get_coordinates_of_square(ground, x, y, &i, &j);
    } while (
        get_square_type_pixels(ground, x, y) == WALL ||
        check_eucledian_distance(player.position, position, 20 * SQUARE_SIZE) == 1 ||
        is_wall_placed_around(ground, i, j, 2) == 1
        );
    guardian->position = position;
    reset_speed_direction(guardian);
    guardian->vision_radius = GUARDIAN_VISION_RADIUS;
}

/**
 * @brief Initialize guardians
 *
 * @param guardians
 * @param ground
 * @param player
 * @param count
 */
void init_guardians(Guardians* guardians, Ground ground, Player player, int count) {
    int i;
    guardians->count = count;
    guardians->guardians = malloc(count * sizeof(Guardian));
    if (guardians->guardians == NULL) {
        fprintf(stderr, "Error: malloc failed in init_guardians\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < count; i++) {
        init_guardian(&guardians->guardians[i], ground, player);
    }
    guardians->panic = 0;
    guardians->panic_start = 0;
}

/**
 * @brief Free guardians
 *
 * @param guardians
 */
void free_guardians(Guardians* guardians) {
    free(guardians->guardians);
}

/**
 * @brief Update the position of a guardian
 *
 * @param guardian
 * @param new
 * @param new_panic
 * @param panic
 */
static void update_position(Guardian guardian, Position* new, Position* new_panic, int panic) {
    switch (guardian.direction) {
    case UP:
        new->y -= guardian.speed;
        if (panic) new_panic->y = new->y - SQUARE_SIZE;
        break;
    case DOWN:
        new->y += guardian.speed;
        if (panic) new_panic->y = new->y + SQUARE_SIZE;
        break;
    case LEFT:
        new->x -= guardian.speed;
        if (panic) new_panic->x = new->x - SQUARE_SIZE;
        break;
    case RIGHT:
        new->x += guardian.speed;
        if (panic) new_panic->x = new->x + SQUARE_SIZE;
        break;
    case NONE:
        break;
    default:
        break;
    }

}

/**
 * @brief Move a guardian
 *
 * @param guardian
 * @param ground
 * @param panic
 */
static void move_guardian(Guardian* guardian, Ground ground, int panic) {
    Position new_position = guardian->position;
    Position new_position_panic = guardian->position;

    /* update position */
    update_position(*guardian, &new_position, &new_position_panic, panic);

    /* check collision and validate new position */
    if (check_collision(panic ? new_position_panic : new_position, ground)) {
        reset_speed_direction(guardian);
    } else if (rand() % 50 == 0) {
        reset_speed_direction(guardian);
    } else {
        guardian->position = new_position;
    }
}

/**
 * @brief Move guardians
 *
 * @param guardians
 * @param ground
 */
void move_guadians(Guardians* guardians, Ground ground) {
    int i;
    for (i = 0; i < guardians->count; i++) {
        move_guardian(&guardians->guardians[i], ground, guardians->panic);
    }
}

/**
 * @brief Return the euclidean distance between two positions
 *
 * @param p0
 * @param p1
 * @return double
 */
static double euclidean_distance(Position p0, Position p1) {
    return sqrt(pow(p0.x - p1.x, 2) + pow(p0.y - p1.y, 2));
}

/**
 * @brief Check if a position is in the vision of a guardian
 * and if the guardian can see the other position (no wall)
 *
 * @param ground
 * @param p0
 * @param p1
 * @return int
 */
static int is_vision_clear_x(Ground ground, Position p0, Position p1) {
    double x, y;
    int i, p0i, p1i, p0j, p1j;

    get_coordinates_of_square(ground, p0.x, p0.y, &p0i, &p0j);
    get_coordinates_of_square(ground, p1.x, p1.y, &p1i, &p1j);

    if (p0i > p1i) {
        for (i = p1i; i <= p0i; i++) {
            x = i * SQUARE_SIZE;
            y = p0.y + (p1.y - p0.y) * (x - p0.x) / (p1.x - p0.x);
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 0;
            }
        }
    } else {
        for (i = p0i; i <= p1i; i++) {
            x = i * SQUARE_SIZE;
            y = p0.y + (p1.y - p0.y) * (x - p0.x) / (p1.x - p0.x);
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 0;
            }
        }
    }

    return 1;
}

/**
 * @brief Check if a position is in the vision of a guardian
 * and if the guardian can see the other position (no wall)
 *
 * @param ground
 * @param p0
 * @param p1
 * @return int
 */
static int is_vision_clear_y(Ground ground, Position p0, Position p1) {
    double x, y;
    int j, p0i, p1i, p0j, p1j;

    get_coordinates_of_square(ground, p0.x, p0.y, &p0i, &p0j);
    get_coordinates_of_square(ground, p1.x, p1.y, &p1i, &p1j);

    if (p0j > p1j) {
        for (j = p1j; j <= p0j; j++) {
            y = j * SQUARE_SIZE;
            x = p0.x + (p1.x - p0.x) * (y - p0.y) / (p1.y - p0.y);
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 0;
            }
        }
    } else {
        for (j = p0j; j <= p1j; j++) {
            y = j * SQUARE_SIZE;
            x = p0.x + (p1.x - p0.x) * (y - p0.y) / (p1.y - p0.y);
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 0;
            }
        }
    }

    return 1;
}

/**
 * @brief Return 1 if a position is in the vision of a guardian
 *
 * @param guardian
 * @param position
 * @param ground
 * @return int
 */
static int is_position_in_guardian_vision(Guardian* guardian, Position position, Ground ground) {
    Position p0, p1;
    if (euclidean_distance(guardian->position, position) > guardian->vision_radius) {
        return 0;
    }

    p0.x = position.x;
    p0.y = position.y;
    p1.x = guardian->position.x;
    p1.y = guardian->position.y;

    if (is_vision_clear_x(ground, p0, p1) == 0) {
        return 0;
    }

    if (is_vision_clear_y(ground, p0, p1) == 0) {
        return 0;
    }

    return 1;
}

/**
 * @brief Return 1 if a player is in the vision of a guardian
 *
 * @param guardian
 * @param player
 * @param ground
 * @return int
 */
int is_player_in_guardians_vision(Guardians* guardians, Player player, Ground ground) {
    int i;
    if (player.skill == INVISIBILITY) {
        return 0;
    }
    for (i = 0; i < guardians->count; i++) {
        if (is_position_in_guardian_vision(&guardians->guardians[i], player.position, ground)) {
            return 1;
        }
    }
    return 0;
}

/**
 * @brief Return 1 if a relic is in the vision of a guardian and if the guardian can see the relic (no wall)
 * It changes the type of the square where the relic is to RELIC_TAKEN_AND_SEEN
 *
 * @param ground
 * @param guardians
 * @param nb
 * @param i_list
 * @param j_list
 * @return int
 */
static int check_relic_detection(Ground* ground, Guardians* guardians, int nb, int* i_list, int* j_list) {
    int i, j, x, y;
    Position relic_position;
    for (i = 0; i < guardians->count; i++) {
        for (j = 0; j < nb; j++) {
            get_position_of_square(*ground, i_list[j], j_list[j], &x, &y);
            relic_position.x = x;
            relic_position.y = y;
            relic_position.x += SQUARE_SIZE / 2;
            relic_position.y += SQUARE_SIZE / 2;
            if (is_position_in_guardian_vision(&guardians->guardians[i], relic_position, *ground)) {
                ground->squares[i_list[j]][j_list[j]].square = RELIC_TAKEN_AND_SEEN;
                return 1;
            }
        }
    }
    return 0;
}

/**
 * @brief Return 1 if a relic is in the vision of a guardian and if the guardian can see the relic (no wall)
 * Return 0 otherwise
 *
 * @param ground
 * @param guardians
 * @param nb
 * @return int
 */
static int relic_taken_detected(Ground* ground, Guardians* guardians, int nb) {
    int* i_list, * j_list, res;
    i_list = (int*)malloc(sizeof(int) * nb);
    if (i_list == NULL) {
        return 0;
    }
    j_list = (int*)malloc(sizeof(int) * nb);
    if (j_list == NULL) {
        return 0;
    }
    get_coords_square(*ground, i_list, j_list, RELIC_TAKEN);

    res = check_relic_detection(ground, guardians, nb, i_list, j_list);

    free(i_list);
    free(j_list);
    return res;
}

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
int check_panic(Guardians* guardians, Player player, Ground* ground, double chrono) {
    int i, nb_relics_taken;
    nb_relics_taken = get_nb_square(*ground, RELIC_TAKEN);
    if (relic_taken_detected(ground, guardians, nb_relics_taken) == 1) {
        guardians->panic = 1;
        guardians->panic_start = chrono;
        return 1;
    }
    if (guardians->panic) {
        if (chrono - guardians->panic_start > PANIC_DURATION) {
            guardians->panic = 0;
            for (i = 0; i < guardians->count; i++) {
                reset_speed_direction(&guardians->guardians[i]);
                guardians->guardians[i].vision_radius = GUARDIAN_VISION_RADIUS;
            }
            return -1;
        }
        for (i = 0; i < guardians->count; i++) {
            guardians->guardians[i].vision_radius = GUARDIAN_VISION_RADIUS_PANIC;
            guardians->guardians[i].speed = SPEED;
        }
    }
    return 0;
}