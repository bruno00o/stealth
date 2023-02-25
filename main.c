/**
 * @file main.c
 * @author SEILLIEBERT BRUNO
 * @brief Main file of the Stealth game
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

#include "include/game.h"

 /**
  * @brief Display the help
  *
  * @param argc
  * @param argv
  * @return int
  */
int options(int argc, char** argv) {
    int opt;
    char c;
    FILE* readme;
    struct option long_options[] = {
        {"help", no_argument, 0, 'h'},
        {0, 0, 0, 0}
    };
    while ((opt = getopt_long(argc, argv, "h", long_options, NULL)) != -1) {
        switch (opt) {
        case 'h':
            readme = fopen("README.md", "r");
            if (readme == NULL) {
                fprintf(stderr, "Error: fopen failed in options\n");
                return 0;
            }
            while ((c = fgetc(readme)) != EOF) {
                printf("%c", c);
            }
            fclose(readme);
            return 0;
        }
    }
    return 1;
}

/**
 * @brief Main function
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char const* argv[]) {
    Game game;
    if (!options(argc, (char**)argv)) {
        return 0;
    }

    init_game(&game);
    play_game(&game);
    free_game(&game);

    return 0;
}
