/**
 * @file display.h
 * @author SEILLIEBERT BRUNO
 * @brief Display functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __DISPLAY__
#define __DISPLAY__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <MLV/MLV_all.h>

#include "ground.h"
#include "player.h"
#include "guardian.h"
#include "leaderboard.h"

 /**
  * @brief Structure containing all the sounds of the game
  *
  */
typedef struct {
    MLV_Sound* relic;
    MLV_Sound* selected;
    MLV_Sound* mana;
    MLV_Sound* loose;
    MLV_Sound* win;
    MLV_Sound* panic;
    MLV_Sound* skill;
    MLV_Sound* skill_remove;
} Sounds;

/**
 * @brief Create the MLV window using a default font
 *
 */
void init_display();

/**
 * @brief Initialize the sounds
 *
 * @param sounds
 */
void init_sounds(Sounds* sounds);

/**
 * @brief Display the game on the screen
 *
 * @param ground
 * @param player
 */
void display_game(Ground ground, Player player, Guardians guardians, double chrono);

/**
 * @brief Display the start screen
 * Only two options : start the game (SPACE) or quit (ESCAPE)
 * Return 1 if the player wants to start the game, 0 if he wants to quit
 *
 * @param message
 * @return int
 */
int display_start(char* message, Sounds sounds);

/**
 * @brief Display the loose screen
 *
 * @param chrono
 * @param mana_spent
 * @param leaderboard
 */
void display_loose(double chrono, int mana_spent, Leaderboard leaderboard);

/**
 * @brief Display the win screen and ask for a name
 *
 * @param chrono
 * @param mana_spent
 * @param leaderboard
 * @param desactive
 * @return char*
 */
char* display_win(double chrono, int mana_spent, Leaderboard leaderboard, int desactive_input);

/**
 * @brief Play the relic found sound
 *
 * @param sounds
 */
void relic_found_sound(Sounds* sounds);

/**
 * @brief Play mana sound
 *
 * @param sounds
 */
void mana_sound(Sounds* sounds);

/**
 * @brief Play the loose sound
 *
 * @param sounds
 */
void loose_sound(Sounds* sounds);

/**
 * @brief Play the win sound
 *
 * @param sounds
 */
void win_sound(Sounds* sounds);

/**
 * @brief Play the panic sound
 *
 * @param sounds
 */
void panic_sound(Sounds* sounds);

/**
 * @brief Play the skill sound
 *
 * @param sounds
 */
void skill_sound(Sounds* sounds);

/**
 * @brief Play the skill remove sound
 *
 * @param sounds
 */
void skill_remove_sound(Sounds* sounds);

/**
 * @brief Free the sounds
 *
 * @param sounds
 */
void free_sounds(Sounds* sounds);

#endif