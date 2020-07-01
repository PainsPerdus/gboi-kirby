#include <gb/gb.h>

#include "enemy_monster.h"
#include "dummy.sprites.h"

#include "global.h"

#include "player.sprites.h"
#include "chainsaw_lateral.sprites.h"
#include "chainsaw_vertical.sprites.h"
#include "animations.h"
#include "oam_pool.h"


// Initialize an enemy unit
void init_enemy(ENEMY* unit, UINT8 enemy_sprite_l, UINT8 enemy_sprite_r, UINT8 attack_type, UINT8 damage, UINT8 hp, UINT8 frames_between_attacks) {
	unit->enemy_sprite_l = enemy_sprite_l;
	unit->enemy_sprite_r = enemy_sprite_r;
	unit->sprite_id1 = borrow_oam_id();
	unit->sprite_id2 = borrow_oam_id();
	unit->attack_type = attack_type;
	unit->max_health = hp;
	unit->health = hp; // Enemies always start full HP.
	unit->damage = damage;
	unit->frames_between_attacks = frames_between_attacks;
	unit->frames_until_next_attack = frames_between_attacks; // There is one full cycle before the enemy starts behaving normally. Might want to be able to configure that.
	unit->frames_until_next_step = WALKING_FRAMES_BETWEEN_STEPS;
	unit->walking_animation_state = 0;
	unit->walking_direction = WALKING_DIRECTION_LEFT;
	unit->dying_animation_state = 0; // Start off alive.
}

inline void init_melee(ENEMY* unit) {
    init_enemy(unit, DUMMY_SPRITE_ID, DUMMY_SPRITE_ID + 2, ENEMY_ATTACK_SELF, 2, 8, 30); // The fourth parameter is stopgap! Might want #define or sprite id pool
}

// Display enemy unit on-screen at specified x and y coordinates
void display_enemy(ENEMY* unit, UINT8 xpos, UINT8 ypos) {	
	// Initialize left sprite
	set_sprite_tile(unit->sprite_id1, unit->enemy_sprite_l);
	
	// Initialize right sprite
	set_sprite_tile(unit->sprite_id2, unit->enemy_sprite_r);
	
	// Place the sprites
	move_enemy(unit, xpos, ypos);
}

// Handles absolute enemy movement to specified (x,y) coordinates
void move_enemy(ENEMY* unit, UINT8 xpos, UINT8 ypos) {
	// Moves enemy unit to (x,y)
	move_sprite(unit->sprite_id1, xpos + X_SPRITE_OFFSET + scroll_x, ypos + Y_SPRITE_OFFSET + scroll_y);
	move_sprite(unit->sprite_id2, xpos + X_SPRITE_OFFSET + 8 + scroll_x, ypos + Y_SPRITE_OFFSET + scroll_y);
	
	// Store current position unless it's out of range
	if (xpos < X_OFFSCREEN && ypos < Y_OFFSCREEN) {
		unit->xpos = xpos;
		unit->ypos = ypos;
	}
}

// NOTE: won't work in big rooms!
// Handles relative enemy movement by (dx,dy)
void scroll_enemy(ENEMY* unit, INT8 dxpos, INT8 dypos) {
	// Moves enemy unit by (dx,dy)
	scroll_sprite(unit->sprite_id1, dxpos, dypos);
	scroll_sprite(unit->sprite_id2, dxpos, dypos);

	unit->xpos += dxpos;
	unit->ypos += dypos;
}

// Handles all steps of the enemy death sequence
void enemy_death(ENEMY* unit) {
	switch(unit->dying_animation_state) {
		case 1: // Make the enemy disappear temporarily
		case 13:
		case 25:
		case 37:
			move_enemy(unit, X_OFFSCREEN, Y_OFFSCREEN);
			break;
		case 7: // Make the enemy reappear temporarily
		case 19:
		case 31:
		case 43:
			move_enemy(unit, unit->xpos, unit->ypos); // This will work because move_enemy does not update (x, y) if it's equal to (200, 200)
			break;
		case 49: // Enemy disappears, for real this time!
			enemy_release(unit);

			break;
		default:
			break;
	}
	
	if (unit->dying_animation_state < 49)
		unit->dying_animation_state++;
}


void enemy_release(ENEMY* unit) {
	move_enemy(unit, X_OFFSCREEN, Y_OFFSCREEN);
	free_oam_id(unit->sprite_id1);
	free_oam_id(unit->sprite_id2);
}

// Enemy unit loses specified amount of HP
void enemy_hp_loss(ENEMY* unit, UINT8 amount) {
	if (amount < unit->health) { // The unit survives, so it just loses the specified amount of HP
		unit->health -= amount;
		// Optional: might want a sound here
	} else { // The unit dies: bring its HP to 0 and then call enemy_death
		unit->health = 0;
		// Make the death animation triggerable in the main program
		unit->dying_animation_state = 1;
	}
	
	// Enable palette swap
	set_sprite_prop(unit->sprite_id1, get_sprite_prop(unit->sprite_id1) | S_PALETTE);
	set_sprite_prop(unit->sprite_id2, get_sprite_prop(unit->sprite_id2) | S_PALETTE);
	// Wait 1/30th of a second
	wait_vbl_done();
	wait_vbl_done();
	// Swap to default palette
	set_sprite_prop(unit->sprite_id1, get_sprite_prop(unit->sprite_id1) & ~S_PALETTE);
	set_sprite_prop(unit->sprite_id2, get_sprite_prop(unit->sprite_id2) & ~S_PALETTE);
}

// Enemy unit gains specified amount of HP. This is capped to max enemy health.
void enemy_hp_regen(ENEMY* unit, UINT8 amount) {
	if (unit->health) { // Do not heal a dead enemy
		if (unit->health + amount <= unit->max_health) { // Gain intended amount
			unit->health += amount;
		} else { // Cap it to max health 
			unit->health = unit->max_health;
		}
	}
}

// Enemy unit launches its attack #TODO: handle MELEE/PROJECTILE
void handle_enemy_attack(ENEMY* unit) {
	if (unit->health) { // Check: an enemy can only attack if it has nonzero health
		unit->frames_until_next_attack--;
		if (!unit->frames_until_next_attack) { // It's attacking time!
			switch (unit->attack_type) {
				case ENEMY_ATTACK_SELF:
					enemy_hp_loss(unit, unit->damage);
					// Optional: might want to load sound!
					// Reset frame counter until next attack
					unit->frames_until_next_attack = unit->frames_between_attacks;
					break;
			
				// Todo: handle projectile launch and melee attacks. In the meantime, nothing happens.
				case ENEMY_ATTACK_MELEE: // #TODO: detect whether the player's in melee range, then attack it
				case ENEMY_ATTACK_PROJECTILE: // #TODO: can only attack horizontally or vertically
				case ENEMY_ATTACK_NONE:
				default:
					unit->frames_until_next_attack = unit->frames_between_attacks;
					break;
			}
		}
	}
}

// Handle enemy walking sequence
// #TODO: checks: ensure we're not going out of bounds (should I assume AI cannot change rooms, unlike players), mind collisions (objects, player)
void handle_enemy_walk(ENEMY* unit) {
	unit->frames_until_next_step--;
	
	if (!unit->frames_until_next_step) {
		switch (unit->walking_direction) {
			case WALKING_DIRECTION_DOWN:
				if (unit->ypos < 120) // Arbitrary value for testing purposes; should be replaced with an actual "can go ahead" check
					scroll_enemy(unit, 0, 1);
				break;
			
			case WALKING_DIRECTION_UP:
				if (unit->ypos > 0) // Ditto
					scroll_enemy(unit, 0, -1);
				break;
			
			case WALKING_DIRECTION_RIGHT:
				if (unit->xpos < 120) // Ditto
					scroll_enemy(unit, 1, 0);
				break;
				
			case WALKING_DIRECTION_LEFT:
				if (unit->xpos > 0) // Ditto
					scroll_enemy(unit, -1, 0);
				break;
			
			default:
				break;
		}
		
		// Reset waiting sequence
		unit->frames_until_next_step = WALKING_FRAMES_BETWEEN_STEPS;
	}
}


ENEMY enemy_stack[MAX_ENEMY_NB];
UINT8 enemy_stack_ptr = 0;

ENEMY* get_available_enemy() {
	ENEMY* enemy = &enemy_stack[enemy_stack_ptr];
	enemy_stack_ptr ++;
	if (enemy_stack_ptr == MAX_ENEMY_NB)  // cycle back
		enemy_stack_ptr = 0;
	return enemy;
}