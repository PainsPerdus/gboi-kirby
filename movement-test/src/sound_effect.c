#include <gb/gb.h>
#include "sound_effect.h"

void init_sound() {

	__asm__("ld a, #0x80"); 	// $1000 000 : enable sound register
	__asm__("ld (#0xFF26),a");
	__asm__("ld a, #0x77"); 	// $0111 0111 : S01 and SO2 to max volume
	__asm__("ld (#0xFF24),a");

}


void play_falling_sound() {

	init_sound();
	__asm__("ld a, #0x11"); 	// $0001 0001 : S01 and S02 use only sound 1
	__asm__("ld (#0xFF25),a");
	__asm__("ld a, #0xFF"); 	// $1111 1111 : envelope decrease
	__asm__("ld (#0xFF10),a");
	__asm__("ld a, #0x40"); 	// $0100 0000 : wave pattern
	__asm__("ld (#0xFF11),a");
	__asm__("ld a, #0xF7"); 	// $1111 0111 : envelope volume
	__asm__("ld (#0xFF12),a");
	__asm__("ld a, #0xA9"); 	// $1010 1001 : frequency part 1
	__asm__("ld (#0xFF13),a");
	__asm__("ld a, #0xC7"); 	// $1100 0111 : initial frequency = 1500 Hz
	__asm__("ld (#0xFF14),a");
}

void play_hit_sound() {

	init_sound();
	__asm__("ld a,#0x99"); 		// $1001 1001 : S01 and S02 use sound 1 and 4
	__asm__("ld (#0xFF25),a");
	        // Channel 1
	__asm__("ld a, #0x9E"); 	// $1001 1110 : envelope decrease
	__asm__("ld (#0xFF10),a");
	__asm__("ld a,#0x94"); 		// $1001 0100 : wave pattern
	__asm__("ld (#0xFF11),a");
	__asm__("ld a,#0xA0");		// $1010 0000 : envelope volume
	__asm__("ld (#0xFF12),a");
	__asm__("ld a,#0xAC"); 		// $1010 1100 : frequency part 1
	__asm__("ld (#0xFF13),a");
	__asm__("ld a,#0xC5"); 		// $1100 0101 : initial frequency = 1048 Hz
	__asm__("ld (#0xFF14),a");
		// Channel 4
	__asm__("ld a,#0x38");		// $0011 1000 : length : smaller as channel 1
	__asm__("ld (#0xFF20),a");
	__asm__("ld a,#0xB7"); 		// $1011 0111 :volume of envelope is lower
	__asm__("ld (#0xFF21),a");
	__asm__("ld a,#0x70"); 		// $0111 0000 : configure the frequency sweep
	__asm__("ld (#0xFF22),a");
	__asm__("ld a,#0xC0"); 		// $1100 0000 : start the channel
	__asm__("ld (#0xFF23),a");
}

void play_dash_sound() {

	init_sound();
        __asm__("ld a,#0x88"); 		// $1000 1000 : S01 and S02 use only sound 4
        __asm__("ld (#0xFF25),a");
        __asm__("ld a,#0x10"); 		// $0001 0000 : length : maximum
        __asm__("ld (#0xFF20),a");
        __asm__("ld a,#0x2A"); 		// $0010 1010 : volume of envelope increase
        __asm__("ld (#0xFF21),a");
        __asm__("ld a,#0x33"); 		// $0011 0011 : configure the frequency sweep
        __asm__("ld (#0xFF22),a");
        __asm__("ld a,#0xC0"); 		// $1100 0000 : start the channel
        __asm__("ld (#0xFF23),a");
}

void play_chainsaw_attack_sound() {

	init_sound();
        __asm__("ld a,#0x88"); 		// $1000 1000 : S01 and S02 use only sound 4
        __asm__("ld (#0xFF25),a");
        __asm__("ld a,#0x0E"); 		// $0000 1110 : Sound length
        __asm__("ld (#0xFF20),a");
        __asm__("ld a,#0xCF"); 		// $1100 1111 : envellope volume
        __asm__("ld (#0xFF21),a");
        __asm__("ld a,#0x63"); 		// 0110 0011 : frequency changes
        __asm__("ld (#0xFF22),a");
        __asm__("ld a,#0xC0"); 		// 1100 0000 : init the sound
        __asm__("ld (#0xFF23),a");
}
