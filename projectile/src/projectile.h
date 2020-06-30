#include <gb/gb.h>

#ifndef _PROJECTILE_H
#define _PROJECTILE_H

#define FIRING_DIRECTION_LEFT 0
#define FIRING_DIRECTION_RIGHT 1
#define FIRING_DIRECTION_UP 2
#define FIRING_DIRECTION_DOWN 3

#define TARGET_PLAYER 0
#define TARGET_MONSTER 1
#define TARGET_ANY 2

#ifndef _SPRITE_OFFSET
#define X_SPRITE_OFFSET 8
#define Y_SPRITE_OFFSET 16
#endif

typedef struct projectile {
	UINT8 sprite_tile; // sprite ID in the tileset
	UINT8 sprite_id; // sprite ID (0-39)
	UINT8 firing_direction; // see FIRING_DIRECTION_* macros
	UINT8 xpos;
	UINT8 ypos;
	UINT8 velocity; // projectile movement speed in pixels per frame
	UINT8 targets; // see TARGET_* macros
	UINT8 damage; // damage the projectile should do. This will depend on the unit firing it.
} PROJECTILE;

/**
 * @brief Initializes a projectile
 *
 * @param p projectile
 * @param s_id sprite ID (0-39)
 * @param s_tile projectile tile
 * @param targets
 * @param damage
 * @param velocity
 */
void projectile_init(PROJECTILE* p, UINT8 s_id, UINT8 s_tile, UINT8 targets, UINT8 damage, UINT8 velocity);

/**
 * @brief Place projectile at specified (x,y) coordinates, and define its direction (see FIRING_DIRECTION_* macros)
 *
 * @param p
 * @param xpos
 * @param ypos
 * @param direction see FIRING_DIRECTION_* macros
 */
void projectile_display(PROJECTILE* p, UINT8 xpos, UINT8 ypos, UINT8 direction);
#endif