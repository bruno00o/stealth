/**
 * @file keys.c
 * @author SEILLIEBERT BRUNO
 * @brief Keys functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#include "../include/keys.h"

 /**
  * @brief Initialize the keys structure
  * In this function, we initialize the keys structure with all the keys of the keyboard
  * and the state of each key to MLV_RELEASED
  *
  * @param keys
  */
void init_keys(Keys* keys) {
    int i;
    keys->size = MLV_KEYBOARD_LAST;
    keys->keys = malloc(keys->size * sizeof(Key));
    if (keys->keys == NULL) {
        fprintf(stderr, "Error: malloc failed in init_keys\n");
        exit(EXIT_FAILURE);
    }
    for (i = 0; i < keys->size; i++) {
        keys->keys[i].key = *(MLV_Keyboard_button*)&i;
        keys->keys[i].state = MLV_RELEASED;
    }
}

/**
 * @brief Free the keys structure
 *
 * @param keys
 */
void free_keys(Keys* keys) {
    free(keys->keys);
}

/**
 * @brief Get the pressed keys
 *
 * @param keys
 * @param size
 * @return MLV_Keyboard_button*
 */
void get_pressed_keys(MLV_Keyboard_button* pressed_keys, Keys keys, int* size) {
    int i;
    int j = 0;
    for (i = 0; i < keys.size; i++) {
        if (keys.keys[i].state == MLV_PRESSED) {
            pressed_keys[j] = keys.keys[i].key;
            j++;
        }
    }
    *size = j;
}
