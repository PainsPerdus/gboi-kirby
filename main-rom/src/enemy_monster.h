#include <gb/gb.h>

#ifndef _ENEMY_MONSTER_H
#define _ENEMY_MONSTER_H

#define ENEMY_ATTACK_PROJECTILE 0
#define ENEMY_ATTACK_MELEE 1
#define ENEMY_ATTACK_SELF 2 // used for testing purposes
#define ENEMY_ATTACK_NONE 3 // innocent monster

#define WALKING_FRAMES_BETWEEN_STEPS 1

#define WALKING_DIRECTION_LEFT 0
#define WALKING_DIRECTION_RIGHT 1
#define WALKING_DIRECTION_UP 2
#define WALKING_DIRECTION_DOWN 3
#define WALKING_DIRECTION_IMMOBILE 4

// sprite placement offset
#ifndef _SPRITE_OFFSET
#define X_SPRITE_OFFSET 8
#define Y_SPRITE_OFFSET 16
#endif

#define X_OFFSCREEN 240
#define Y_OFFSCREEN 240

typedef struct enemy {
	UINT8 enemy_sprite_l; // enemy left sprite in the tileset
	UINT8 enemy_sprite_r; // enemy right sprite
	UINT8 sprite_id; // left sprite ID (0-38, even number preferrably)
	UINT8 attack_type; // enemy attack type, see macros defined above
	UINT8 max_health; // monster max HP
	UINT8 health; // monster current HP
	UINT8 damage; // attack damage - could be manually changed by boosts to enemy shots
	UINT8 frames_between_attacks; // number of frames between attacks (that is how "attack speed" is controlled)
	UINT8 frames_until_next_attack; // acts as a counter for the current enemy
	UINT8 dying_animation_state; // INTERNAL. 0: enemy alive, 1-49: death animation, above: dead
	UINT8 xpos; // current x pos
	UINT8 ypos; // current y pos
	UINT8 frames_until_next_step; // also a counter, used to handle walking instead
	UINT8 walking_animation_state; // manages walking animation
	UINT8 walking_direction;
	// Unhandled at this point:
	// - projectile speed (if we decide that it depends on the monster rather than, for example, on how far the player got in the game)
} ENEMY;

/**
 * @brief Initializes an enemy unit
 *
 * @param unit
 * @param enemy_sprite_l left sprite
 * @param enemy_sprite_r right sprite
 * @param s_id sprite ID, should be an even number in 0-38
 * @param attack_type above ENEMY_ATTACK_* macros
 * @param damage
 * @param hp
 * @param frames_between_attacks
 */
void init_enemy(ENEMY* unit, UINT8 enemy_sprite_l, UINT8 enemy_sprite_r, UINT8 s_id, UINT8 attack_type, UINT8 damage, UINT8 hp, UINT8 frames_between_attacks);

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
void enemy_death(ENEMY* unit);

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
 * @brief Enemy unit launches its attack
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
#endif