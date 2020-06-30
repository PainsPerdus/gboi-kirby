#include <gb/gb.h>

#include "projectile.h"

// Initialize projectile data
void projectile_init(PROJECTILE* p, UINT8 s_id, UINT8 s_tile, UINT8 targets, UINT8 damage, UINT8 velocity) {
	p->sprite_id = s_id;
	p->sprite_tile = s_tile;
	p->targets = targets;
	p->velocity = velocity;
	p->damage = damage;
}

// Place projectile at specified (x,y) coordinates, and define its direction (see FIRING_DIRECTION_* macros)
void projectile_display(PROJECTILE* p, UINT8 xpos, UINT8 ypos, UINT8 direction) {
	// Store the values
	p->firing_direction = direction;
	p->xpos = xpos;
	p->ypos = ypos;
	
	// Initialize projectile sprite
	set_sprite_tile(p->sprite_id, p->sprite_tile);
	
	// Place the sprite
	move_sprite(p->sprite_id, p->xpos + X_SPRITE_OFFSET, p->ypos + Y_SPRITE_OFFSET);
}