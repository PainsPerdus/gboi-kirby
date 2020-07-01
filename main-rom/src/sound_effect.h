#include <gb/gb.h>

#ifndef _SOUND_EFFECT_H_
#define _SOUND_EFFECT_H_

// Simply call the function when you want the sound to be played
// To know that all functions here calls the init_function (take care of stack overflow)


void init_sound();
// This one wrap all the basic actions to do with sound
// in order to save few lines of codes

/**
 * @brief Play the sound effect of the player falling in a hole
 *
 */
void play_falling_sound();

/**
 * @brief Play the sound effect of the player taking a hit
 *
 */
void play_hit_sound();

/**
 * @brief Play the sound effect of the player doing a dash
 *
 */
void play_dash_sound();

/**
 * @brief Play the chainsaw sound effect for the attack
 *
 */
void play_chainsaw_attack_sound();

#endif

