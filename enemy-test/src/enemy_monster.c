#include <gb/gb.h>

#include "dummy-tileset.h"

#define ENEMY_ATTACK_PROJECTILE 0
#define ENEMY_ATTACK_MELEE 1
#define ENEMY_ATTACK_SELF 2 // used for teting purposes

typedef struct enemy {
	UINT8 enemy_sprite_l; // enemy left sprite in the tileset
	UINT8 enemy_sprite_r; // enemy right sprite
	UINT8 sprite_id; // sprite ID (0-39)
	UINT8 attack_type; // enemy attack type, see constants defined above
	UINT8 max_health; // monster max HP
	UINT8 health; // monster current HP
	UINT8 damage; // attack damage - could be manually changed by boosts to enemy shots
	UINT8 frames_between_attacks; // number of frames between attacks (that is how "attack speed" is controlled)
	UINT8 frames_until_next_attack; // acts as a counter for the current enemy
	// Unhandled at this point:
	// - x/y monster velocity, or whatever way to handle monster speed (to be discussed: how to alter these when a collision occurs? I don't think we want monsters going back and forth along the same straight path all game long)
	// - projectile speed (if we decide that it depends on the monster rather than, for example, on how far the player got in the game)
} ENEMY;

// Temporary background tileset for testing purposes
const UINT8 BKG_TILESET[] = {
    // Tile 00: Blank
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    // Tile 01: Block
    0xff, 0x01, 0x81, 0x7f, 0xbd, 0x7f, 0xa5, 0x7b,
    0xa5, 0x7b, 0xbd, 0x63, 0x81, 0x7f, 0xff, 0xff
};

// Temporary tilemap for testing purposes
const UINT8 BKG_TILEMAP[] = {
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1
};

// Initialize an enemy unit
void init_enemy(ENEMY *unit, UINT8 enemy_sprite_l, UINT8 enemy_sprite_r, UINT8 s_id, UINT8 attack_type, UINT8 damage, UINT8 hp, UINT8 fba) {
	unit->enemy_sprite_l = enemy_sprite_l;
	unit->enemy_sprite_r = enemy_sprite_r;
	unit->sprite_id = s_id;
	unit->attack_type = attack_type;
	unit->max_health = hp;
	unit->health = hp; // Enemies always start full HP.
	unit->damage = damage;
	unit->frames_between_attacks = fba;
	unit->frames_until_next_attack = fba; // There is one full cycle before the enemy starts behaving normally. Might want to be able to configure that.
}

// (To be tested) Display enemy unit on-screen, with specified sprite number (0-39), at specified x and y coordinates.
void display_enemy(ENEMY *unit, UINT8 xpos, UINT8 ypos) {
	// Initialize left sprite
	set_sprite_tile(unit->sprite_id, unit->enemy_sprite_l);
	move_sprite(unit->sprite_id, xpos, ypos);
	
	// Initialize right sprite (#TODO: check whether the number parameter is correctly handled)
	set_sprite_tile(unit->sprite_id + 1, unit->enemy_sprite_r);
	move_sprite(unit->sprite_id + 1, xpos + 8, ypos);
}

// Play death sequence (blinking, presumably), then make the enemy disappear
void enemy_death(ENEMY *unit) {
	// Blinking animation - disabled for now
	/*
	UINT8 blinking_cycles = 0;
	UINT8 cycle_state = 1;
	
	while (blinking_cycles < 10)
	{
		wait_vbl_done();
		
		if (cycle_state) // Disappears
		{
			move_sprite(unit->sprite_id, );
			cycle_state = 0;
		}
		else
		{
			move_sprite();
		}
	}
	*/
	
	// Enemy disappears
	move_sprite(unit->sprite_id, 168, 0);
	move_sprite(unit->sprite_id + 1, 168, 0);
}

// Enemy unit loses specified amount of HP
void enemy_hp_loss(ENEMY *unit, UINT8 amount) {
	if (amount < unit->health) // The unit survives, so it just loses the specified amount of HP
	{
		unit->health -= amount;
		// Optional: might want to shout "ouch" or something
	}
	else // The unit dies: bring its HP to 0 and then call enemy_death
	{
		unit->health = 0;
		enemy_death(unit);
	}
}

// Enemy unit gains specified amount of HP. This is capped to max enemy health. (To be tested.)
void enemy_hp_regen(ENEMY *unit, UINT8 amount) {
	if (unit->health + amount <= unit->max_health) // Gain intended amount
	{
		unit->health += amount;
	}
	else
	{
		unit->health = unit->max_health;
	}
}

// Enemy unit launches its attack #TODO: handle MELEE/PROJECTILE
void enemy_attack(ENEMY *unit) {
	if (unit->health) // Check: an enemy can only attack if it has nonzero health
	{
		switch (unit->attack_type)
		{
			case ENEMY_ATTACK_SELF:
				enemy_hp_loss(unit, unit->damage);
				// Optional: might want to load sound!
				break;
			
			// Todo: handle projectile launch and melee attacks. In the meantime, nothing happens.
			case ENEMY_ATTACK_MELEE: // #TODO: detect whether the player's in melee range, then attack it
			case ENEMY_ATTACK_PROJECTILE: // #TODO: can only attack horizontally or vertically
			default:
				break;
		}
	}
}

void main(void) {
    set_bkg_data(0, 2, BKG_TILESET);
    set_bkg_tiles(0, 0, 20, 18, BKG_TILEMAP);
	set_sprite_data(0, 4, DUMMY_SPRITES);
	ENEMY basic;

	// Loading self-attacking enemy (two attacks/second)
	init_enemy(&basic, 0, 2, 0, ENEMY_ATTACK_SELF, 13, 2, 30);
	display_enemy(&basic, 72, 80);
	SHOW_BKG;
	SPRITES_8x16;
	SHOW_SPRITES;
	
	while(1) {
		wait_vbl_done(); // Wait screen refresh
		
		// Proceed by one frame and check whether the enemy can attack
		basic.frames_until_next_attack--;
		if (!basic.frames_until_next_attack) {
			// Attack and reset frame countdown
			enemy_attack(&basic);
			basic.frames_until_next_attack = basic.frames_between_attacks;
		}
	}
}