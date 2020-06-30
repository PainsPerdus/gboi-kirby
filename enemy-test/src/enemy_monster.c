#include <gb/gb.h>

#include "enemy_monster.h"
#include "dummy.sprites.h"

// Initialize an enemy unit
void init_enemy(ENEMY* unit, UINT8 enemy_sprite_l, UINT8 enemy_sprite_r, UINT8 s_id, UINT8 attack_type, UINT8 damage, UINT8 hp, UINT8 frames_between_attacks) {
	unit->enemy_sprite_l = enemy_sprite_l;
	unit->enemy_sprite_r = enemy_sprite_r;
	unit->sprite_id = s_id;
	unit->attack_type = attack_type;
	unit->max_health = hp;
	unit->health = hp; // Enemies always start full HP.
	unit->damage = damage;
	unit->frames_between_attacks = frames_between_attacks;
	unit->frames_until_next_attack = frames_between_attacks; // There is one full cycle before the enemy starts behaving normally. Might want to be able to configure that.
}

// Display enemy unit on-screen at specified x and y coordinates
void display_enemy(ENEMY* unit, UINT8 xpos, UINT8 ypos) {
	// Store current position
	unit->xpos = xpos;
	unit->ypos = ypos;
	
	// Initialize left sprite
	set_sprite_tile(unit->sprite_id, unit->enemy_sprite_l);
	
	// Initialize right sprite
	set_sprite_tile(unit->sprite_id + 1, unit->enemy_sprite_r);
	
	// Place the sprite
	move_enemy(unit, xpos, ypos);
}

void move_enemy(ENEMY* unit, UINT8 xpos, UINT8 ypos) {
	// Moves enemy unit to (x,y)
	move_sprite(unit->sprite_id, xpos + 8, ypos + 16);
	move_sprite(unit->sprite_id + 1, xpos + 16, ypos + 16);
}

// Play death sequence (blinking), then make the enemy disappear
void enemy_death(ENEMY* unit) {
	// Blinking animation - disabled
	/*
	UINT8 blinking_cycles = 0;
	UINT8 cycle_state = 1;
	
	UINT8 former_ypos = unit->ypos; // Retain former y pos for the blinking animation
	
	while (blinking_cycles < 10) {	
		if (cycle_state) { // Disappears
			move_sprite(unit->sprite_id, unit->xpos, 0);
			move_sprite(unit->sprite_id+1, unit->xpos+8, 0);
			cycle_state = 0;
		} else {
			move_sprite(unit->sprite_id, unit->xpos, former_ypos);
			move_sprite(unit->sprite_id+1, unit->xpos+8, former_ypos);
			cycle_state = 1;
			blinking_cycles++;
		}
	}*/
	
	// Enemy disappears
	move_enemy(unit, 168, 0);
	// #TODO: release that sprite id in the sprite id pool once said pool is created
}

// Enemy unit loses specified amount of HP
void enemy_hp_loss(ENEMY* unit, UINT8 amount) {
	if (amount < unit->health) { // The unit survives, so it just loses the specified amount of HP
		unit->health -= amount;
		// Optional: might want to shout "ouch" or something
	} else { // The unit dies: bring its HP to 0 and then call enemy_death
		unit->health = 0;
		enemy_death(unit);
	}
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
void enemy_attack(ENEMY* unit) {
	if (unit->health) { // Check: an enemy can only attack if it has nonzero health
		switch (unit->attack_type) {
			case ENEMY_ATTACK_SELF:
				enemy_hp_loss(unit, unit->damage);
				// Optional: might want to load sound!
				break;
			
			// Todo: handle projectile launch and melee attacks. In the meantime, nothing happens.
			case ENEMY_ATTACK_MELEE: // #TODO: detect whether the player's in melee range, then attack it
			case ENEMY_ATTACK_PROJECTILE: // #TODO: can only attack horizontally or vertically
			case ENEMY_ATTACK_INNOCENT:
			default:
				break;
		}
		
		// Reset frame counter until next attack
		unit->frames_until_next_attack = unit->frames_between_attacks;
	}
}