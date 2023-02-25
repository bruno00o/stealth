/**
 * @file keys.h
 * @author SEILLIEBERT BRUNO
 * @brief Keys functions
 * @version 0.1
 * @date 2023-01-13
 *
 * @copyright Copyright (c) 2023
 *
 */

#ifndef __KEYS__
#define __KEYS__

#include <stdio.h>
#include <stdlib.h>

#include <MLV/MLV_all.h>

 /**
  * @brief Key structure
  *
  */
typedef struct {
    MLV_Keyboard_button key;
    MLV_Button_state state;
} Key;

/**
 * @brief Keys list structure
 *
 */
typedef struct {
    Key* keys;
    int size;
} Keys;

/**
 * @brief Initialize the keys structure
 * In this function, we initialize the keys structure with all the keys of the keyboard
 * and the state of each key to MLV_RELEASED
 *
 * @param keys
 */
void init_keys(Keys* keys);

/**
 * @brief Free the keys structure
 *
 * @param keys
 */
void free_keys(Keys* keys);

/**
 * @brief Get the pressed keys
 *
 * @param keys
 * @param size
 * @return MLV_Keyboard_button*
 */
void get_pressed_keys(MLV_Keyboard_button* pressed_keys, Keys keys, int* size);

#endif