#include <gb/gb.h>

#include "projectile.h"

// Initialize projectile data
void projectile_init(PROJECTILE* p, UINT8 s_id, UINT8 s_tile, UINT8 targets, UINT8 damage, UINT8 travel_freq) {
	p->sprite_id = s_id;
	p->sprite_tile = s_tile;
	p->targets = targets;
	p->travel_freq = travel_freq;
	p->frames_until_next_travel = travel_freq;
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

// Should be triggered when changing frames, for each projectile (might want to hold a list of them throughout the game)
void projectile_travel(PROJECTILE* p) {
	p->frames_until_next_travel--;
	
	if (!frames_until_next_travel) { 
		switch (p->firing_direction) {
			case FIRING_DIRECTION_DOWN:
				scroll_sprite(p->sprite_id, 0, 1);
				p->ypos += 1;
				break;
			case FIRING_DIRECTION_UP:
				scroll_sprite(p->sprite_id, 0, -1);
				p->ypos -= 1;
				break;
			case FIRING_DIRECTION_RIGHT:
				scroll_sprite(p->sprite_id, 1, 0);
				p->xpos += 1;
				break;
			case FIRING_DIRECTION_LEFT:
				scroll_sprite(p->sprite_id, -1, 0);
				p->xpos -= 1;
				break;
			default: // Stand still
				break;
		}
		
		p->frames_until_next_travel = p->travel_freq;
	}
}