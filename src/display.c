/**
 * @file display.c
 * @author SEILLIEBERT BRUNO
 * @brief Display functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/display.h"

#ifndef M_PI
#    define M_PI 3.14159265358979323846
#endif

#ifndef BASIC_CIRCLE
 /**
  * @brief Check if there is a wall between two points on the x axis
  *
  * @param ground
  * @param p0
  * @param p1
  * @return int
  */
static int is_wall_between_x(Ground ground, Position p0, Position p1) {
    double x, y;
    int i, p0i, p1i, p0j, p1j;

    get_coordinates_of_square(ground, p0.x, p0.y, &p0i, &p0j);
    get_coordinates_of_square(ground, p1.x, p1.y, &p1i, &p1j);

    if (p0i > p1i) {
        for (i = p1i; i <= p0i; i++) {
            x = i * SQUARE_SIZE;
            if (p0.x == p1.x) {
                y = p0.y;
            } else {
                y = p0.y + (p1.y - p0.y) * (x - p0.x) / (p1.x - p0.x);
            }
            if (x < 0 || x > ground.pixel_width || y < 0 || y > ground.pixel_height) {
                continue;
            }
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 1;
            }
        }
    } else {
        for (i = p0i; i <= p1i; i++) {
            x = i * SQUARE_SIZE;
            if (p0.x == p1.x) {
                y = p0.y;
            } else {
                y = p0.y + (p1.y - p0.y) * (x - p0.x) / (p1.x - p0.x);
            }
            if (x < 0 || x > ground.pixel_width || y < 0 || y > ground.pixel_height) {
                continue;
            }
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * @brief Check if there is a wall between two points on the y axis
 *
 * @param ground
 * @param p0
 * @param p1
 * @return int
 */
static int is_wall_between_y(Ground ground, Position p0, Position p1) {
    double x, y;
    int j, p0i, p1i, p0j, p1j;

    get_coordinates_of_square(ground, p0.x, p0.y, &p0i, &p0j);
    get_coordinates_of_square(ground, p1.x, p1.y, &p1i, &p1j);

    if (p0j > p1j) {
        for (j = p1j; j <= p0j; j++) {
            y = j * SQUARE_SIZE;
            if (p0.y == p1.y) {
                x = p0.x;
            } else {
                x = p0.x + (p1.x - p0.x) * (y - p0.y) / (p1.y - p0.y);
            }
            if (x < 0 || x > ground.pixel_width || y < 0 || y > ground.pixel_height) {
                continue;
            }
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 1;
            }
        }
    } else {
        for (j = p0j; j <= p1j; j++) {
            y = j * SQUARE_SIZE;
            if (p0.y == p1.y) {
                x = p0.x;
            } else {
                x = p0.x + (p1.x - p0.x) * (y - p0.y) / (p1.y - p0.y);
            }
            if (x < 0 || x > ground.pixel_width || y < 0 || y > ground.pixel_height) {
                continue;
            }
            if (get_square_type_pixels(ground, x, y) == WALL) {
                return 1;
            }
        }
    }

    return 0;
}

/**
 * @brief Draw the radius of the guardian vision
 *
 * @param guardian
 * @param ground
 * @param color
 */
static void draw_guardian_radius(Guardian guardian, Ground ground, MLV_Color color) {
    int j, is_wall, count;
    double angle, x, y;
    int* x_list, * y_list;
    Position p0, p1;
    angle = 0;

    p1.x = guardian.position.x;
    p1.y = guardian.position.y;

    x_list = malloc(sizeof(int) * 200);
    y_list = malloc(sizeof(int) * 200);
    count = 0;

    for (angle = 0; angle < 2 * M_PI; angle += 0.17) {
        is_wall = 0;
        p0.x = p1.x + guardian.vision_radius * cos(angle);
        p0.y = p1.y + guardian.vision_radius * sin(angle);

        is_wall = is_wall_between_x(ground, p0, p1);
        if (!is_wall) {
            is_wall = is_wall_between_y(ground, p0, p1);
        }

        x = p0.x;
        y = p0.y;

        if (is_wall) {
            for (j = 0; j < guardian.vision_radius; j += 2) {
                x = guardian.position.x + j * cos(angle);
                y = guardian.position.y + j * sin(angle);
                if (get_square_type_pixels(ground, x, y) == WALL) {
                    break;
                }
            }
        }

        x_list[count] = x;
        y_list[count] = y;

        count++;
    }

    MLV_draw_filled_polygon(x_list, y_list, count, color);

    free(x_list);
    free(y_list);
}
#else
 /**
  * @brief Draw the radius of the guardian vision
  *
  * @param guardian
  * @param ground
  * @param color
  */
static void draw_guardian_radius(Guardian guardian, Ground ground, MLV_Color color) {
    MLV_draw_circle(guardian.position.x, guardian.position.y, guardian.vision_radius, MLV_COLOR_ORANGERED3);
    MLV_draw_circle(guardian.position.x, guardian.position.y, guardian.vision_radius - 1, MLV_COLOR_ORANGERED3);
}
#endif


/**
 * @brief Create the MLV window using a default font
 *
 */
void init_display() {
    MLV_create_window_with_default_font("Stealth", "Stealth", GROUND_WIDTH * SQUARE_SIZE, GROUND_HEIGHT * SQUARE_SIZE, "assets/PressStart2P-Regular.ttf", SQUARE_SIZE * 1.5);
    if (MLV_init_audio() == -1) {
        fprintf(stderr, "Can't initialize audio system.\n");
        exit(EXIT_FAILURE);
    }
    MLV_change_number_of_parallel_sounds(20);
}

/**
 * @brief Initialize the sounds
 *
 * @param sounds
 */
void init_sounds(Sounds* sounds) {
    sounds->relic = MLV_load_sound("assets/relic.wav");
    sounds->selected = MLV_load_sound("assets/selected.wav");
    sounds->mana = MLV_load_sound("assets/mana.wav");
    sounds->loose = MLV_load_sound("assets/loose.wav");
    sounds->win = MLV_load_sound("assets/win.wav");
    sounds->panic = MLV_load_sound("assets/panic.wav");
    sounds->skill = MLV_load_sound("assets/skill.wav");
    sounds->skill_remove = MLV_load_sound("assets/skill_remove.wav");
}

/**
 * @brief Display the ground on the screen
 *
 * @param ground
 */
static void display_ground(Ground ground) {
    int i, j;
    for (i = 0; i < GROUND_HEIGHT; i++) {
        for (j = 0; j < GROUND_WIDTH; j++) {
            switch (ground.squares[j][i].square) {
            case WALL:
                MLV_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, MLV_COLOR_GREY12);
                break;
            case RELIC:
                MLV_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, MLV_COLOR_GOLD);
                break;
            case RELIC_TAKEN:
            case RELIC_TAKEN_AND_SEEN:
                MLV_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, MLV_COLOR_DARK_GOLDENROD);
                break;
            case FLOOR:
                MLV_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, MLV_COLOR_CORNSILK);
                break;
            case STARTING_FLOOR:
                MLV_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, MLV_COLOR_SEA_GREEN);
                break;
            case MANA:
                MLV_draw_filled_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, MLV_COLOR_DARK_SEA_GREEN);
                break;
            }
            /* draw borders */
            MLV_draw_rectangle(j * SQUARE_SIZE, i * SQUARE_SIZE, SQUARE_SIZE, SQUARE_SIZE, MLV_COLOR_GREY12);
        }
    }
}

/**
 * @brief Display the player on the screen
 *
 * @param player
 */
static void display_player(Player player) {
    MLV_Color color = MLV_rgba(0, 98, 255, 255);
    if (player.skill == SPEED_UP) {
        color = MLV_COLOR_GREEN;
    } else if (player.skill == INVISIBILITY) {
        color = MLV_rgba(0, 98, 255, 80);
    }
    MLV_draw_filled_circle(player.position.x, player.position.y, SQUARE_SIZE / 2, color);
}

/**
 * @brief Display the guardians on the screen
 *
 * @param guardians
 * @param ground
 * @param chrono
 */
static void display_guardians(Guardians guardians, Ground ground, double chrono) {
    int i, d;
    MLV_Color color_panic = MLV_rgba(255, 0, 0, 40);
    for (i = 0; i < guardians.count; i++) {
        MLV_draw_filled_circle(guardians.guardians[i].position.x, guardians.guardians[i].position.y, SQUARE_SIZE / 2, MLV_COLOR_ORANGERED3);
        draw_guardian_radius(guardians.guardians[i], ground, color_panic);

    }

    if (guardians.panic) {
        d = (int)chrono;
        if (d % 2 == 0) {
            MLV_draw_filled_rectangle(0, 0, ground.pixel_width, ground.pixel_height, color_panic);
        }
    }
}

/**
 * @brief Display the game on the screen
 *
 * @param ground
 * @param player
 */
void display_game(Ground ground, Player player, Guardians guardians, double chrono) {
    MLV_clear_window(MLV_COLOR_BLACK);
    display_ground(ground);
    display_player(player);
    display_guardians(guardians, ground, chrono);
    MLV_actualise_window();
}

/**
 * @brief Display the start screen
 * Only two options : start the game (SPACE) or quit (ESCAPE)
 * Return 1 if the player wants to start the game, 0 if he wants to quit
 *
 * @param message
 * @return int
 */
int display_start(char* message, Sounds sounds) {
    MLV_Keyboard_button key;
    MLV_Color color = MLV_rgba(200, 200, 200, 180);
    MLV_Color transparent = MLV_rgba(0, 0, 0, 0);
    MLV_Font* title = MLV_load_font("assets/PressStart2P-Regular.ttf", SQUARE_SIZE * 3);
    MLV_draw_filled_rectangle(0, 0, GROUND_WIDTH * SQUARE_SIZE, GROUND_HEIGHT * SQUARE_SIZE, color);
    MLV_draw_text_box_with_font(0, 0, GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2 - 100, "Stealth", title, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_BOTTOM);
    MLV_draw_text_box(0, (GROUND_HEIGHT * SQUARE_SIZE) / 2, GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2, message, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    MLV_actualise_window();
    MLV_free_font(title);
    do {
        MLV_wait_keyboard(&key, NULL, NULL);
    } while (key != MLV_KEYBOARD_SPACE && key != MLV_KEYBOARD_ESCAPE);
    if (key == MLV_KEYBOARD_SPACE) {
        MLV_play_sound(sounds.selected, 1);
        return 1;
    }
    return 0;
}

/**
 * @brief Display the leaderboard
 *
 * @param leaderboard
 */
static void display_leaderboard(Leaderboard leaderboard) {
    int i;
    MLV_Color transparent = MLV_rgba(0, 0, 0, 0);
    MLV_Font* title = MLV_load_font("assets/PressStart2P-Regular.ttf", SQUARE_SIZE * 1.2);
    MLV_Font* lines = MLV_load_font("assets/PressStart2P-Regular.ttf", SQUARE_SIZE * 1);
    MLV_draw_filled_rectangle(0, (GROUND_HEIGHT * SQUARE_SIZE) / 2 - (SQUARE_SIZE * 2), GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2, MLV_COLOR_WHITE);
    MLV_draw_text_box_with_font(0, (GROUND_HEIGHT * SQUARE_SIZE) / 2, (GROUND_WIDTH * SQUARE_SIZE) / 2, (GROUND_HEIGHT * SQUARE_SIZE) / 2, "Leaderboard Chrono", title, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    MLV_draw_text_box_with_font((GROUND_WIDTH * SQUARE_SIZE) / 2, (GROUND_HEIGHT * SQUARE_SIZE) / 2, (GROUND_WIDTH * SQUARE_SIZE) / 2, (GROUND_HEIGHT * SQUARE_SIZE) / 2, "Leaderboard Mana", title, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    for (i = 0; i < NB_SCORES; i++) {
        MLV_draw_text_box_with_font(SQUARE_SIZE * 4, (GROUND_HEIGHT * SQUARE_SIZE) / 2 + (SQUARE_SIZE * 3) + (SQUARE_SIZE * 1.5 * i), (GROUND_WIDTH * SQUARE_SIZE) / 2, (GROUND_HEIGHT * SQUARE_SIZE) / 2, "%d. %s : %f", lines, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_LEFT, MLV_VERTICAL_TOP, i + 1, (strcmp(leaderboard.chrono[i].name, "") == 0) ? "......." : leaderboard.chrono[i].name, leaderboard.chrono[i].chrono == MAX_CHRONO ? 0.000 : leaderboard.chrono[i].chrono);
        MLV_draw_text_box_with_font((GROUND_WIDTH * SQUARE_SIZE) / 2 + (SQUARE_SIZE * 4), (GROUND_HEIGHT * SQUARE_SIZE) / 2 + (SQUARE_SIZE * 3) + (SQUARE_SIZE * 1.5 * i), (GROUND_WIDTH * SQUARE_SIZE) / 2, (GROUND_HEIGHT * SQUARE_SIZE) / 2, "%d. %s : %d", lines, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_LEFT, MLV_VERTICAL_TOP, i + 1, (strcmp(leaderboard.mana[i].name, "") == 0) ? "......." : leaderboard.mana[i].name, leaderboard.mana[i].mana == MAX_MANA ? 0 : leaderboard.mana[i].mana);
    }
    MLV_free_font(title);
    MLV_free_font(lines);
}


/**
 * @brief Display the loose screen
 *
 * @param chrono
 * @param mana_spent
 * @param leaderboard
 */
void display_loose(double chrono, int mana_spent, Leaderboard leaderboard) {
    char message[100];
    MLV_Keyboard_button key;
    MLV_Color color = MLV_rgba(200, 200, 200, 200);
    MLV_Color transparent = MLV_rgba(0, 0, 0, 0);
    MLV_Font* title = MLV_load_font("assets/PressStart2P-Regular.ttf", SQUARE_SIZE * 2);
    sprintf(message, "Chrono: %f, Mana spent: %d", chrono, mana_spent);
    MLV_draw_filled_rectangle(0, 0, GROUND_WIDTH * SQUARE_SIZE, GROUND_HEIGHT * SQUARE_SIZE, color);
    MLV_draw_text_box_with_font(0, (SQUARE_SIZE * 5), GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2 - (SQUARE_SIZE * 5), "You loose!", title, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    MLV_draw_text_box(0, (SQUARE_SIZE * 10), GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2, "Press space to quit", 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    MLV_draw_text_box(0, (SQUARE_SIZE * 15), GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2, message, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    display_leaderboard(leaderboard);
    MLV_actualise_window();
    MLV_free_font(title);
    do {
        MLV_wait_keyboard_or_mouse(&key, NULL, NULL, NULL, NULL);
    } while (key != MLV_KEYBOARD_SPACE && key != MLV_KEYBOARD_ESCAPE);
}

/**
 * @brief Display the win screen
 *
 * @param chrono
 * @param mana_spent
 * @param input
 * @param leaderboard
 */
static void display_win_screen(double chrono, int mana_spent, MLV_Input_box* input, Leaderboard leaderboard) {
    char message[100];
    MLV_Color transparent = MLV_rgba(0, 0, 0, 0);
    MLV_Font* title = MLV_load_font("assets/PressStart2P-Regular.ttf", SQUARE_SIZE * 2);

    sprintf(message, "Chrono: %f, Mana spent: %d", chrono, mana_spent);

    MLV_draw_text_box_with_font(0, (SQUARE_SIZE * 5), GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2 - (SQUARE_SIZE * 5), "You win!", title, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    MLV_draw_text_box(0, (SQUARE_SIZE * 10), GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2, "Press space to quit", 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    MLV_draw_text_box(0, (SQUARE_SIZE * 15), GROUND_WIDTH * SQUARE_SIZE, (GROUND_HEIGHT * SQUARE_SIZE) / 2, message, 0, transparent, MLV_COLOR_BLACK, transparent, MLV_TEXT_CENTER, MLV_HORIZONTAL_CENTER, MLV_VERTICAL_TOP);
    MLV_draw_all_input_boxes();
    display_leaderboard(leaderboard);
    MLV_actualise_window();

    MLV_free_font(title);

}

/**
 * @brief Display the win screen and ask for a name
 *
 * @param chrono
 * @param mana_spent
 * @param leaderboard
 * @param desactive
 * @return char*
 */
char* display_win(double chrono, int mana_spent, Leaderboard leaderboard, int desactive) {
    MLV_Keyboard_button key;
    MLV_Event event;
    char* texte, * texte1;
    MLV_Color color = MLV_rgba(200, 200, 200, 180);
    MLV_Font* input_font = MLV_load_font("assets/PressStart2P-Regular.ttf", SQUARE_SIZE * 1);
    MLV_Input_box* input = MLV_create_input_box_with_font((GROUND_WIDTH * SQUARE_SIZE) / 18, (SQUARE_SIZE * 17.5), (GROUND_WIDTH * SQUARE_SIZE) - (GROUND_WIDTH * SQUARE_SIZE) / 10, 50, MLV_COLOR_BLACK, MLV_COLOR_BLACK, MLV_COLOR_WHITE, "  Name:", input_font);
    if (desactive == 1) {
        MLV_desactivate_input_box();
    } else {
        MLV_draw_filled_rectangle(0, 0, GROUND_WIDTH * SQUARE_SIZE, GROUND_HEIGHT * SQUARE_SIZE, color);
    }
    texte1 = (char*)malloc(1 * sizeof(char)); *texte1 = '\0';

    display_win_screen(chrono, mana_spent, input, leaderboard);
    do {
        event = MLV_get_event(&key, NULL, NULL, &texte, &input, NULL, NULL, NULL, NULL);
        if (event == MLV_INPUT_BOX) {
            free(texte1);
            texte1 = texte;
            return texte1;
        }
        display_win_screen(chrono, mana_spent, input, leaderboard);
    } while (key != MLV_KEYBOARD_SPACE && key != MLV_KEYBOARD_ESCAPE);
    MLV_free_font(input_font);
    return NULL;
}

/**
 * @brief Play the relic found sound
 *
 * @param sounds
 */
void relic_found_sound(Sounds* sounds) {
    MLV_play_sound(sounds->relic, 1);
}

/**
 * @brief Play mana sound
 *
 * @param sounds
 */
void mana_sound(Sounds* sounds) {
    MLV_play_sound(sounds->mana, .25);
}

/**
 * @brief Play the loose sound
 *
 * @param sounds
 */
void loose_sound(Sounds* sounds) {
    MLV_play_sound(sounds->loose, 1);
}

/**
 * @brief Play the win sound
 *
 * @param sounds
 */
void win_sound(Sounds* sounds) {
    MLV_play_sound(sounds->win, 1);
}

/**
 * @brief Play the panic sound
 *
 * @param sounds
 */
void panic_sound(Sounds* sounds) {
    MLV_play_sound(sounds->panic, .25);
}

/**
 * @brief Play the skill sound
 *
 * @param sounds
 */
void skill_sound(Sounds* sounds) {
    MLV_play_sound(sounds->skill, .1);
}

/**
 * @brief Play the skill remove sound
 *
 * @param sounds
 */
void skill_remove_sound(Sounds* sounds) {
    MLV_play_sound(sounds->skill_remove, .1);
}

/**
 * @brief Free the sounds
 *
 * @param sounds
 */
void free_sounds(Sounds* sounds) {
    MLV_free_sound(sounds->relic);
    MLV_free_sound(sounds->selected);
    MLV_free_sound(sounds->mana);
    MLV_free_sound(sounds->loose);
    MLV_free_sound(sounds->win);
    MLV_free_sound(sounds->panic);
    MLV_free_sound(sounds->skill);
    MLV_free_sound(sounds->skill_remove);
}