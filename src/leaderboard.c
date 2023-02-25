/**
 * @file leaderboard.c
 * @author SEILLIEBERT BRUNO
 * @brief Leaderboard functions
 * @version 0.1
 * @date 2023-01-13
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#include "../include/leaderboard.h"

/**
 * @brief Save the leaderboard in a file
 * 
 * @param leaderboard 
 */
static void leaderboard_save(const Leaderboard* leaderboard) {
    FILE* file = fopen(LEADERBOARD_PATH, "wb");
    if (file == NULL) {
        fprintf(stderr, "Error: fopen failed in leaderboard_save\n");
        exit(EXIT_FAILURE);
    }
    fwrite(leaderboard->chrono, sizeof(ScoreChrono), NB_SCORES, file);
    fwrite(leaderboard->mana, sizeof(ScoreMana), NB_SCORES, file);
    fclose(file);
}

/**
 * @brief Add a score to the leaderboard
 * 
 * @param leaderboard 
 * @param name 
 * @param chrono 
 * @param mana 
 */
void leaderboard_add_score(Leaderboard* leaderboard, char* name, double chrono, int mana) {
    int i;
    if (strlen(name) >= NAME_LENGTH) {
        name[NAME_LENGTH - 1] = '\0';
    }
    for (i = 0; i < NB_SCORES; i++) {
        if (chrono < leaderboard->chrono[i].chrono) {
            memmove(leaderboard->chrono + i + 1, leaderboard->chrono + i, (NB_SCORES - i - 1) * sizeof(ScoreChrono));
            strcpy(leaderboard->chrono[i].name, name);
            leaderboard->chrono[i].chrono = chrono;
            break;
        }
    }
    for (i = 0; i < NB_SCORES; i++) {
        if (mana < leaderboard->mana[i].mana) {
            memmove(leaderboard->mana + i + 1, leaderboard->mana + i, (NB_SCORES - i - 1) * sizeof(ScoreMana));
            strcpy(leaderboard->mana[i].name, name);
            leaderboard->mana[i].mana = mana;
            break;
        }
    }
    leaderboard_save(leaderboard);
}

/**
 * @brief Load the leaderboard from a file
 * 
 * @param leaderboard 
 */
static void leaderboard_load(Leaderboard* leaderboard) {
    FILE* file = fopen(LEADERBOARD_PATH, "rb");
    if (file == NULL) {
        return;
    }
    if (!fread(leaderboard->chrono, sizeof(ScoreChrono), NB_SCORES, file)) {
        fprintf(stderr, "Error: fread failed in leaderboard_load\n");
        exit(EXIT_FAILURE);
    }
    if (!fread(leaderboard->mana, sizeof(ScoreMana), NB_SCORES, file)) {
        fprintf(stderr, "Error: fread failed in leaderboard_load\n");
        exit(EXIT_FAILURE);
    }
    fclose(file);
}

/**
 * @brief Initialize the leaderboard
 * If the leaderboard file doesn't exist, create it and fill it with default values
 * 
 * @param leaderboard 
 */
void init_leaderboard(Leaderboard* leaderboard) {
    FILE* file;
    int i;
    file = fopen(LEADERBOARD_PATH, "rb");
    if (file == NULL) {
        file = fopen(LEADERBOARD_PATH, "wb");
        if (file == NULL) {
            fprintf(stderr, "Error: fopen failed in init_leaderboard\n");
            exit(EXIT_FAILURE);
        }
        for (i = 0; i < NB_SCORES; i++) {
            strcpy(leaderboard->chrono[i].name, "");
            leaderboard->chrono[i].chrono = MAX_CHRONO;
            strcpy(leaderboard->mana[i].name, "");
            leaderboard->mana[i].mana = MAX_MANA;
        }
        fwrite(leaderboard->chrono, sizeof(ScoreChrono), NB_SCORES, file);
        fwrite(leaderboard->mana, sizeof(ScoreMana), NB_SCORES, file);
        fclose(file);
    } else {
        fclose(file);
    }

    leaderboard_load(leaderboard);
}
