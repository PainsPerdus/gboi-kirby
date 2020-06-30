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

typedef struct projectile {
	UINT8 sprite_tile; // sprite ID in the tileset
	UINT8 sprite_id; // sprite ID (0-39)
	UINT8 firing_direction; // see FIRING_DIRECTION_* macros
	UINT8 xpos;
	UINT8 ypos;
	UINT8 velocity;
	UINT8 targets; // see TARGET_* macros
	UINT8 damage;
} PROJECTILE;

#endif