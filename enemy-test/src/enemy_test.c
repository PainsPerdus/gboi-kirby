#include <gb/gb.h>

#include "enemy_monster.h"
#include "dummy.sprites.h"
#include "tileset.h"

// Using the same test map as in movement-test branch
const UINT8 TILEMAP[18*18] = {
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,4,
    4,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
};

void main(void) {
    set_bkg_data(0, TILESET_TILE_COUNT, TILESET);
    set_bkg_tiles(2, 0, 18, 18, TILEMAP);
	set_sprite_data(0, 4, DUMMY_SPRITES);
	ENEMY basic;

	// Loading self-attacking enemy (two attacks/second)
	init_enemy(&basic, 0, 2, 0, ENEMY_ATTACK_SELF, 2, 8, 60);
	display_enemy(&basic, 72, 80);
	
	SHOW_BKG;
	SPRITES_8x16;
	SHOW_SPRITES;
	
	while(1) {
		wait_vbl_done(); // Wait screen refresh
		
		// Proceed by one frame and check whether the enemy can attack
		basic.frames_until_next_attack--;
		if (!basic.frames_until_next_attack) {
			// For testing purposes: regenerate 1 HP before each self-attack, while alive.
			enemy_hp_regen(&basic, 1);
			
			// Attack and reset frame countdown
			enemy_attack(&basic);
			basic.frames_until_next_attack = basic.frames_between_attacks;
		}
	}
}