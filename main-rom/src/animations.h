#ifndef _ANIMATIONS_H
#define _ANIMATIONS_H

/**
 * Sprite groups
**/

// Group 1: player
#define PLAYER_SPRITE_ID 0

#define FIRST_PLAYER_SPRITE PLAYER_SPRITE_ID


// Group 2: lateral chainsaw
#define CHAINSAW_TOP_LATERAL_SPRITE_ID (FIRST_PLAYER_SPRITE + PLAYER_SPRITES_TILE_COUNT)
#define CHAINSAW_WOOSH_1_LATERAL_SPRITE_ID (CHAINSAW_TOP_LATERAL_SPRITE_ID + 2)
#define CHAINSAW_WOOSH_2_LATERAL_SPRITE_ID (CHAINSAW_WOOSH_1_LATERAL_SPRITE_ID + 2)


#define FIRST_CHAINSAW_LATERAL_SPRITE CHAINSAW_TOP_LATERAL_SPRITE_ID


// Group 3: vertical chainsaw
#define CHAINSAW_VERTICAL_SPRITE_ID (FIRST_CHAINSAW_LATERAL_SPRITE + CHAINSAW_LATERAL_SPRITES_TILE_COUNT)

#define FIRST_CHAINSAW_VERTICAL_SPRITE CHAINSAW_VERTICAL_SPRITE_ID


// Group 4: dummy
#define DUMMY_SPRITE_ID (FIRST_CHAINSAW_VERTICAL_SPRITE + CHAINSAW_VERTICAL_SPRITES_TILE_COUNT)

#define FIRST_DUMMY_SPRITE DUMMY_SPRITE_ID


/**
 * End sprite groups.
**/


// Animations data for the player's sprite
static UINT8 PLAYER_SPRITE_ANIM[] = {
// LEN   FLIP            TILES ID         DIRECTION
   4,    0, 0, 1, 1,     0, 2,  0,  2,           // Down
   4,    0, 0, 1, 1,     4, 6,  4,   6,   // Up
   4,    0, 0, 0, 0,     8, 10, 8, 10,   // Right
   4,    1, 1, 1, 1,     8, 10, 8, 10,   // Left
   4,    0, 0, 0, 0,     16, 16, 16, 16,   // dash down
   4,    0, 0, 0, 0,     12, 12, 12, 12,   // dash up
   4,    0, 0, 0, 0,     14, 14, 14, 14,   // right
   4,    1, 1, 1, 1,     14, 14, 14, 14,   // Left
};

// Offset of the animation of each direction in the global animation data
#define PLAYER_DIRECTION_DOWN  0
#define PLAYER_DIRECTION_UP    9
#define PLAYER_DIRECTION_RIGHT 18
#define PLAYER_DIRECTION_LEFT  27
#define PLAYER_DASH_DIRECTION_DOWN 36
#define PLAYER_DASH_DIRECTION_UP 45
#define PLAYER_DASH_DIRECTION_RIGHT 54
#define PLAYER_DASH_DIRECTION_LEFT 63


#endif