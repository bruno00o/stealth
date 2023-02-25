/**
 * @file game.c
 * @author SEILLIEBERT BRUNO
 * @brief Game functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/game.h"


 /**
  * @brief Initialize the game
  *
  * @param game
  */
void init_game(Game* game) {
    srand(time(NULL));
    init_ground(&game->ground);
    init_player(&game->player, game->ground);
    init_guardians(&game->guardians, game->ground, game->player, NB_GUARDIANS);
    init_relics(&game->relics, &game->ground, game->player, NB_RELICS);
    init_leaderboard(&game->leaderboard);
    game->chrono = 0;
    game->state = START;
    init_display();
}

/**
 * @brief Free the game
 *
 * @param game
 */
void free_game(Game* game) {
    free_guardians(&game->guardians);
    free_relics(&game->relics);
}

/**
 * @brief Handles events
 * Returns 0 uf the player pressed escape, 1 otherwise
 *
 * @param game
 * @param keys
 * @param sounds
 * @return int
 */
static int event_handleling(Game* game, Keys* keys, Sounds* sounds) {
    MLV_Event event;
    MLV_Keyboard_button key;
    MLV_Button_state state;

    event = MLV_get_event(&key, NULL, NULL, NULL, NULL, NULL, NULL, NULL, &state);
    if (event == MLV_KEY) {
        keys->keys[key].state = state;
        if (key == MLV_KEYBOARD_ESCAPE) {
            game->state = BREAK;
            return 0;
        }
        if (state == MLV_RELEASED) {
            if (key == MLV_KEYBOARD_UP || key == 'z') {
                stop_player(&game->player);
            } else if (key == MLV_KEYBOARD_DOWN || key == 's') {
                stop_player(&game->player);
            } else if (key == MLV_KEYBOARD_LEFT || key == 'q') {
                stop_player(&game->player);
            } else if (key == MLV_KEYBOARD_RIGHT || key == 'd') {
                stop_player(&game->player);
            } else if (key == MLV_KEYBOARD_LSHIFT || key == MLV_KEYBOARD_RSHIFT) {
                remove_skill(&game->player, SPEED_UP, &game->ground);
                skill_remove_sound(sounds);
            } else if (key == MLV_KEYBOARD_SPACE) {
                remove_skill(&game->player, INVISIBILITY, &game->ground);
                skill_remove_sound(sounds);
            }
        }
    }
    return 1;
}

/**
 * @brief Moves the player and check the use of skills
 *
 * @param game
 * @param keys
 * @param sounds
 */
static void moves(Game* game, Keys* keys, Sounds* sounds) {
    MLV_Keyboard_button* pressed_keys;
    int size, i;

    pressed_keys = malloc(keys->size * sizeof(MLV_Keyboard_button));
    get_pressed_keys(pressed_keys, *keys, &size);

    for (i = 0; i < size; i++) {
        switch (pressed_keys[i]) {
        case MLV_KEYBOARD_UP:
        case 'z':
            move_player(&game->player, UP, game->ground);
            break;
        case MLV_KEYBOARD_DOWN:
        case 's':
            move_player(&game->player, DOWN, game->ground);
            break;
        case MLV_KEYBOARD_LEFT:
        case 'q':
            move_player(&game->player, LEFT, game->ground);
            break;
        case MLV_KEYBOARD_RIGHT:
        case 'd':
            move_player(&game->player, RIGHT, game->ground);
            break;
        case MLV_KEYBOARD_LSHIFT:
        case MLV_KEYBOARD_RSHIFT:
            add_skill(&game->player, SPEED_UP, &game->ground);
            skill_sound(sounds);
            break;
        case MLV_KEYBOARD_SPACE:
            add_skill(&game->player, INVISIBILITY, &game->ground);
            skill_sound(sounds);
            break;
        default:
            break;
        }
    }
    free(pressed_keys);
}

/**
 * @brief Handles panic mode
 *
 * @param game
 * @param sounds
 */
static void panic_handle(Game* game, Sounds* sounds) {
    switch (check_panic(&game->guardians, game->player, &game->ground, game->chrono)) {
    case 0:
        break;
    case 1:
        panic_sound(sounds);
        break;
    case -1:
        MLV_stop_all_sounds();
        break;
    }
}

/**
 * @brief Check if the game is over (win or loose)
 *
 * @param game
 * @return int
 */
static int chek_end_game(Game* game) {
    if (is_player_in_guardians_vision(&game->guardians, game->player, game->ground)) {
        game->state = LOOSE;
        return 1;
    }

    if (is_player_on_starting_floor(game->player, game->ground) && game->player.count_relics == NB_RELICS) {
        game->state = WIN;
        return 1;
    }
    return 0;
}

/**
 * @brief Handles the game state
 * 
 * @param game 
 * @param sounds 
 */
static void handle_game_state(Game* game, Sounds sounds) {
    char* name;
    MLV_stop_all_sounds();
    switch (game->state) {
    case LOOSE:
        loose_sound(&sounds);
        display_loose(game->chrono, game->player.mana_spent, game->leaderboard);
        break;
    case WIN:
        win_sound(&sounds);
        name = display_win(game->chrono, game->player.mana_spent, game->leaderboard, 0);
        if (name != NULL) {
            leaderboard_add_score(&game->leaderboard, name, game->chrono, game->player.mana_spent);
            free(name);
            display_win(game->chrono, game->player.mana_spent, game->leaderboard, 1);
        }
        break;
    case BREAK:
        game->state = display_start("Press space to resume", sounds);
        break;
    default:
        break;
    }
}

/**
 * @brief Play the game
 *
 * @param game
 */
void play_game(Game* game) {
    struct timespec end_time, new_time, chrono_time_start, chrono_time_end;
    double frametime, extratime;
    Keys keys;
    Sounds sounds;

    init_sounds(&sounds);
    init_keys(&keys);
    display_game(game->ground, game->player, game->guardians, game->chrono);

    game->state = display_start("Press space to start", sounds);

    while (game->state == GAME) {
        clock_gettime(CLOCK_REALTIME, &chrono_time_start);
        while (1) {
            clock_gettime(CLOCK_REALTIME, &end_time);
            display_game(game->ground, game->player, game->guardians, game->chrono);

            if (!event_handleling(game, &keys, &sounds)) break;

            move_guadians(&game->guardians, game->ground);
            panic_handle(game, &sounds);
            moves(game, &keys, &sounds);

            if (player_on_mana(&game->player, &game->ground)) mana_sound(&sounds);
            if (relic_found(&game->relics, &game->ground, &game->player)) relic_found_sound(&sounds);

            if (chek_end_game(game)) break;

            clock_gettime(CLOCK_REALTIME, &new_time);

            frametime = new_time.tv_sec - end_time.tv_sec;
            frametime += (new_time.tv_sec - end_time.tv_sec) / 1.0E9;

            extratime = 1.0 / 60 - frametime;
            if (extratime > 0) {
                MLV_wait_milliseconds((int)(extratime * 1000));
            }

            clock_gettime(CLOCK_REALTIME, &chrono_time_end);
            game->chrono += (chrono_time_end.tv_sec - chrono_time_start.tv_sec) + (chrono_time_end.tv_nsec - chrono_time_start.tv_nsec) / 1.0E9;
            clock_gettime(CLOCK_REALTIME, &chrono_time_start);
        }
        handle_game_state(game, sounds);
    }

    free_keys(&keys);
    free_sounds(&sounds);
    MLV_free_window();
}