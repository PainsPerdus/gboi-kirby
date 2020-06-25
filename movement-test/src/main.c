#include <gb/gb.h>
#include <stdio.h>
#include "player.sprites.h"

#define PLAYER_SPRITE_ID 0

// Animations data for the player's sprite
UINT8 PLAYER_SPRITE_ANIM[] = {
// LEN   FLIP            TILES ID         DIRECTION
   4,    0, 0, 1, 1,     0, 2,  0,  2,           // Down
   4,    0, 0, 1, 1,     4, 6,  4,   6,   // Up
   4,    0, 0, 0, 0,     8, 10, 8, 10,   // Right
   4,    1, 1, 1, 1,     8, 10, 8, 10,   // Left
};

// Offset of the animation of each direction in the global animation data
#define PLAYER_DIRECTION_DOWN  0
#define PLAYER_DIRECTION_UP    9
#define PLAYER_DIRECTION_RIGHT 18
#define PLAYER_DIRECTION_RIGHT_DOWN 19  //The diagonal movement has the same animation as the horizontal movement
#define PLAYER_DIRECTION_LEFT  27


// Variables containing player state
UINT8 player_x;
UINT8 player_y;
UINT8 player_direction;
UINT8 player_animation_frame;
UINT8 is_player_walking;

// Flip the given sprite on X axis.
//
// sprite_id: the id ("nb") of the sprite to update.
void flip_sprite_horiz(UINT8 sprite_id) {
    set_sprite_prop(sprite_id, get_sprite_prop(sprite_id) | S_FLIPX);
}

// Remove the flip the given sprite on X axis.
//
// sprite_id: the id ("nb") of the sprite to update.
void unflip_sprite_horiz(UINT8 sprite_id) {
    set_sprite_prop(sprite_id, get_sprite_prop(sprite_id) & ~S_FLIPX);
}

// Update the tiles of the sprite to animate it.
//
// sprite_id: the id ("nb") of the sprite to update
// anim:      pointer to the animation data
// direction: direction of the animation (= offset of the animation in the global animation data)
// frame:     the new frame of the animation that will be displayed
//
// Returns the next frame of the animation.
UINT8 update_sprite_animation(UINT8 sprite_id, UINT8 *anim, UINT8 direction, UINT8 frame) {
    UINT8 len = anim[direction];
    UINT8 flip = anim[direction + 1 + frame]; // +1 : skip length
    UINT8 tile_id = anim[direction + 1 + len + frame]; // +5: skip length+flip data

    if (flip) {
        flip_sprite_horiz(sprite_id);
    } else {
        unflip_sprite_horiz(sprite_id);
    }

    set_sprite_tile(sprite_id, tile_id);

    return (frame + 1) % len;
}

void main(void) {
    UINT8 keys = 0;
    UINT8 frame_skip = 8;  // Update player's animation every 8 frame to
                           // slow down the animation (8 frames = ~133 ms
                           // between each animation frames)

    // Initialize player's state
    player_x = 80;
    player_y = 72;
    player_direction = PLAYER_DIRECTION_DOWN;
    player_animation_frame = 0;
    is_player_walking = 0;

    UINT8 dx = 0;
    UINT8 dy = 0;

    // Load sprites' tiles in video memory
    set_sprite_data(0, PLAYER_SPRITES_TILE_COUNT, PLAYER_SPRITES);

    // Use 8x16 sprites
    SPRITES_8x16;

    // Makes sprites "layer" visible
    SHOW_SPRITES;

    // Init the sprite used for the player
    move_sprite(PLAYER_SPRITE_ID, player_x, player_y);

    // https://gbdev.gg8.se/wiki/articles/GBDK_set_sprite_prop
    set_sprite_prop(PLAYER_SPRITE_ID, 0x00U);


    while (1) {
        // Wait for v-blank (screen refresh)
        wait_vbl_done();

        dx = 0;
        dy = 0;
        // Read joypad keys to know if the player is walking
        // and in which direction
        is_player_walking = 1;
        keys = joypad();
        if (keys & J_UP) {
            player_direction = PLAYER_DIRECTION_UP;
            dy -= 1;
        } else if (keys & J_DOWN) {
            player_direction = PLAYER_DIRECTION_DOWN;
            dy += 1;
        } else if (keys & J_LEFT) {
            player_direction = PLAYER_DIRECTION_LEFT;
            dx -= 1;
        } else if (keys & J_RIGHT ) {
            player_direction = PLAYER_DIRECTION_RIGHT;
            dx += 1;    
        } else {
            is_player_walking = 0;
            frame_skip = 1;  // Force refresh of the animation frame
        }

        // Update the player position if it is walking
        if (is_player_walking) {
            player_x += dx;
            player_y += dy;
            move_sprite(PLAYER_SPRITE_ID, player_x, player_y);

            // We do not update the animation on each frame: the animation
            // will be too quick. So we skip frames
            frame_skip -= 1;
            if (!frame_skip) {
                frame_skip = 8;
            } else {
                continue;
            }
        } else {
            player_animation_frame = 0;
        }


        // Update sprites' tiles
        player_animation_frame = update_sprite_animation(
                PLAYER_SPRITE_ID,
                PLAYER_SPRITE_ANIM,
                player_direction,
                player_animation_frame);
    }
}
