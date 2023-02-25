/**
 * @file leaderboard.h
 * @author SEILLIEBERT BRUNO
 * @brief Leaderboard functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __LEADERBOARD__
#define __LEADERBOARD__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define LEADERBOARD_PATH "leaderboard"
#define NB_SCORES 10
#define NAME_LENGTH 10

#define MAX_CHRONO 1.7976931348623157e+308
#define MAX_MANA 2147483647

 /**
  * @brief Structure of a chrono score
  *
  */
typedef struct {
    char name[NAME_LENGTH];
    double chrono;
} ScoreChrono;

/**
 * @brief Structure of a mana score
 *
 */
typedef struct {
    char name[NAME_LENGTH];
    int mana;
} ScoreMana;

/**
 * @brief Structure of the leaderboard
 *
 */
typedef struct {
    ScoreChrono chrono[NB_SCORES];
    ScoreMana mana[NB_SCORES];
} Leaderboard;

/**
 * @brief Initialize the leaderboard
 * If the leaderboard file doesn't exist, create it and fill it with default values
 *
 * @param leaderboard
 */
void init_leaderboard(Leaderboard* leaderboard);

/**
 * @brief Add a score to the leaderboard
 *
 * @param leaderboard
 * @param name
 * @param chrono
 * @param mana
 */
void leaderboard_add_score(Leaderboard* leaderboard, char* name, double chrono, int mana);

#endif