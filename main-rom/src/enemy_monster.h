#ifndef _ENEMY_MONSTER_H
#define _ENEMY_MONSTER_H

#include <gb/gb.h>
#include "collision.h"

#define ENEMY_ATTACK_PROJECTILE 0
#define ENEMY_ATTACK_MELEE 1
#define ENEMY_ATTACK_SELF 2 // used for testing purposes
#define ENEMY_ATTACK_NONE 3 // innocent monster

#define WALKING_FRAMES_BETWEEN_STEPS 2

#define WALKING_DIRECTION_LEFT 0
#define WALKING_DIRECTION_RIGHT 1
#define WALKING_DIRECTION_UP 2
#define WALKING_DIRECTION_DOWN 3
#define WALKING_DIRECTION_IMMOBILE 4

// enemy sprite placement offset
#define ENEMY_SPRITE_OFFSET_X 8
#define ENEMY_SPRITE_OFFSET_Y 16

#define X_OFFSCREEN 240
#define Y_OFFSCREEN 240

#define ENEMY_HEIGHT 8 // perspective effect
#define ENEMY_WIDTH 16
#define ENEMY_PERSPECTIVE_OFFSET_Y 8

extern RECTANGLE player;
extern const UINT8 ROOM_WIDTH;
extern UINT8 TILEMAP[18*18];

typedef struct enemy {
	UINT8 enemy_sprite_l; // enemy left sprite in the tileset
	UINT8 enemy_sprite_r; // enemy right sprite
	UINT8 sprite_id1; // left sprite ID (0-38, even number preferrably)
	UINT8 sprite_id2; // right sprite ID
	UINT8 attack_type; // enemy attack type, see macros defined above
	UINT8 max_health; // monster max HP
	UINT8 health; // monster current HP
	UINT8 damage; // attack damage - could be manually changed by boosts to enemy shots
	UINT8 frames_between_attacks; // number of frames between attacks (that is how "attack speed" is controlled)
	UINT8 frames_until_next_attack; // acts as a counter for the current enemy
	UINT8 dying_animation_state; // INTERNAL. 0: enemy alive, 1-49: death animation, above: dead
	RECTANGLE enemy_rectangle; // holds enemy current pos
	RECTANGLE enemy_next_rectangle; // used for future pos attempts
	UINT8 frames_until_next_step; // also a counter, used to handle walking instead
	UINT8 walking_animation_state; // manages walking animation
	UINT8 walking_direction; // preferred walking direction
	UINT8 walking_direction_fallback; // fall back to this if the first attempt didn't work
	VEC_DIFF diff_with_player;
	// Unhandled at this point:
	// - projectile speed (if we decide that it depends on the monster rather than, for example, on how far the player got in the game)
} ENEMY;

#define MAX_ENEMY_NB 4
extern ENEMY enemy_stack[MAX_ENEMY_NB];
extern UINT8 enemy_stack_ptr;

/**
 * @brief Initializes an enemy unit
 *
 * @param unit
 * @param enemy_sprite_l left sprite
 * @param enemy_sprite_r right sprite
 * @param attack_type above ENEMY_ATTACK_* macros
 * @param damage
 * @param hp
 * @param frames_between_attacks
 */
void init_enemy(ENEMY* unit, UINT8 enemy_sprite_l, UINT8 enemy_sprite_r, UINT8 attack_type, UINT8 damage, UINT8 hp, UINT8 frames_between_attacks);

/**
 * @brief Initialize a melee unit
 * 
 * @param unit 
 */
void init_melee(ENEMY* unit);

/**
 * @brief Displays an enemy unit on-screen at specified x and y coordinates
 *
 * @param unit
 * @param xpos
 * @param ypos
 */
void display_enemy(ENEMY* unit, UINT8 xpos, UINT8 ypos);

/**
 * @brief Moves an enemy unit to specified x and y coordinates
 *
 * @param unit
 * @param xpos
 * @param ypos
 */
void move_enemy(ENEMY* unit, UINT8 xpos, UINT8 ypos);

/**
 * @brief Moves an enemy unit with specified (dx,dy) movement
 *
 * @param unit
 * @param xpos
 * @param ypos
 */
void scroll_enemy(ENEMY* unit, INT8 dxpos, INT8 dypos);

/**
 * @brief Handles enemy death sequence: death animation and actual death
 *
 * @param unit
 */
void handle_enemy_death(ENEMY* unit);

/**
 * @brief Remove an enemy from the game.
 *
 * @param unit
 */
void enemy_release(ENEMY* unit);

/**
 * @brief Enemy unit loses specified amount of HP
 *
 * @param unit
 * @param amount
 */
void enemy_hp_loss(ENEMY* unit, UINT8 amount);

/**
 * @brief Enemy unit gains specified amount of HP
 *
 * @param unit
 * @param amount
 */
void enemy_hp_regen(ENEMY* unit, UINT8 amount);

/**
 * @brief Handles attacks of specified enemy unit (WIP)
 *
 * @param unit
 */
void handle_enemy_attack(ENEMY* unit);

/**
 * @brief Handles enemy movement (WIP)
 *
 * @param unit
 */
void handle_enemy_walk(ENEMY* unit);

/**
 * @brief Get the next available enemy. If there is none, recycle
 * the oldest.
 * 
 * @return ENEMY* 
 */
ENEMY* get_available_enemy();

/**
 * @brief Calculates and stores x difference and y difference between player and specified unit
 *
 * @param unit
 */
void calculate_diff_with_player(ENEMY* unit);

/**
 * @brief Handles all enemy actions
 *
 * @param unit
 */
void handle_enemy(ENEMY* unit);
#endif