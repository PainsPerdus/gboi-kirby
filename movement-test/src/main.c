#include <gb/gb.h>

#include "player.sprites.h"
#include "collision.h"
#include "tileset.h"

#define PLAYER_SPRITE_ID 0

// Animations data for the player's sprite
UINT8 PLAYER_SPRITE_ANIM[] = {
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

// Variables containing player state
RECTANGLE player;
RECTANGLE new_player;
UINT8 player_direction;
UINT8 player_animation_frame;

#define SPRITE_OFFSET_X 8
#define SPRITE_OFFSET_Y 8  // note: set at 8 even though sprites are 8px tall because of the perspective

#define TILE_SIZE_PX 8
#define MAX_MOVE_X (TILE_SIZE_PX - 1)
#define MAX_MOVE_Y (TILE_SIZE_PX - 1)

static UINT8 cooldown_dash = 200  ;          //cooldown of the dash (in number of frame)  (must be < 255)

const UINT8 ROOM_WIDTH = 16;  // note: should fetch that from the room's data
const UINT8 ROOM_HEIGHT = 16;

INT8 scroll_x = 16;  // initial offset because of the UI
INT8 scroll_y = 0;

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

UINT8 TILEMAP[2][18*18] = {
  {
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
  },
  {
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,4,
    4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4,4
  }
};

static UINT8 effective_x;
static UINT8 effective_y;

static INT8 dx = 0;
static INT8 dy = 0;

static UINT8 keys = 0;
static UINT8 frame_skip = 8; // Update player's animation every 8 frame to
                             // slow down the animation (8 frames = ~133 ms
                             // between each animation frames)

static VEC_DIFF current_diff = {0, 0};
static VEC_DIFF total_diff = {0, 0};


static RECTANGLE block = {{0, 0}, {8, 8}};

static UINT8 longueur_dash;
static UINT8 step1_dash;
static UINT8 step2_dash;
static UINT8 step3_dash;
static UINT8 compteur_dash;
static UINT8 cooldown = 255;

static BOOLEAN is_dashing;


static RECTANGLE left_door_hitbox = {{0, 64}, {7, 16}};
static RECTANGLE right_door_hitbox = {{17 * 8 + 1, 64}, {7, 16}};


static UINT8 room_number = 0;


void load_tilemap() {
    set_bkg_tiles(2, 0, 18, 18, TILEMAP[room_number]);
}


void open_doors() {
  UINT8 change[2] = {0, 0};
  if (room_number == 0) {
    TILEMAP[room_number][8 * 18] = 0;
    TILEMAP[room_number][9 * 18] = 0;
    //set_bkg_tiles(2, 8, 1, 2, change);
    
  //} else if (room_number == 1) {
    TILEMAP[1][9 * 18 - 1] = 0;
    TILEMAP[1][10 * 18 - 1] = 0;
    //set_bkg_tiles(19, 8, 1, 2, change);
  }
  load_tilemap();
}




void read_input() {

    // Read joypad keys to know if the player is walking
    // and in which direction
    keys = joypad();
    if (!is_dashing) {
      if (cooldown < 250){
          cooldown += 1;
      }

      dx = 0;
      dy = 0;
      if (keys & J_UP) {
          player_direction = PLAYER_DIRECTION_UP;
          dy -= 1;
      } if (keys & J_DOWN) {
          player_direction = PLAYER_DIRECTION_DOWN;
          dy += 1;
      } if (keys & J_LEFT) {
          player_direction = PLAYER_DIRECTION_LEFT;
          dx -= 1;
      } if (keys & J_RIGHT) {
          player_direction = PLAYER_DIRECTION_RIGHT;
          dx += 1;
      }

      if(keys & J_SELECT){
        if (cooldown > cooldown_dash){
          player_direction += 36;             //dash tiles = direction tile + 36
          is_dashing = 1;
          compteur_dash = 0;


          dx = dx + dx;
          dy = dy + dy;
          }

      }
    }

}


void init_graphics() {
    // Load sprites' tiles in video memory
    set_sprite_data(0, PLAYER_SPRITES_TILE_COUNT, PLAYER_SPRITES);
    // Use 8x16 sprites
    SPRITES_8x16;
    // Makes sprites "layer" visible
    SHOW_SPRITES;

    set_bkg_data(0, TILESET_TILE_COUNT, TILESET);
    load_tilemap();
    SHOW_BKG;

    // https://gbdev.gg8.se/wiki/articles/GBDK_set_sprite_prop
    set_sprite_prop(PLAYER_SPRITE_ID, 0x00U);


    // XXX: not sure this should be here.
    // Init the sprite used for the player
    // We have to add an offset because of the way the gb handles sprites
    move_sprite(PLAYER_SPRITE_ID, player.pos.x + SPRITE_OFFSET_X + scroll_x, player.pos.y + SPRITE_OFFSET_Y + scroll_y);

}


void init_player_state() {
    // XXX: it's wrong to just hardcode the player's position like that.
    player.pos.x = 80;
    player.pos.y = 72;

    player.size.w = 8;
    player.size.h = 8;

    // XXX: still not sure about new_player's precise role.
    // Is it the properties the player will have during the next frame?
    // This is not really useful yet.
    new_player.size.w = player.size.w;
    new_player.size.h = player.size.h;

    player_direction = PLAYER_DIRECTION_DOWN;
    player_animation_frame = 0;
}



#define MAX(x,y) ((x) < (y) ? (y) : (x))
#define MIN(x,y) ((x) < (y) ? (x) : (y))
#define CLAMP(x, minVal, maxVal) MIN(MAX(minVal, x), maxVal)
#define ABS_CLAMP(x, absMaxVal) CLAMP(x, -absMaxVal, absMaxVal)


void handle_dash() {
  // TODO for dash: use Nolwenn's sprites.
  // TODO: add a cooldown.

  // If the player is dashing, he can't control his movement anymore
  // after x iterations of the dashing code, we set is_dashing to 0 and that ends the dash
  if (is_dashing) {
    longueur_dash = 12;
    step1_dash = 4;
    step2_dash = 7;
    step3_dash = 9;

    // The length of the dash is reduced if the movement is diagonal.
    if (dx != 0 && dy != 0){
      longueur_dash = 8;
      step1_dash = 2;
      step2_dash = 4;
      step3_dash = 6;
    }

    // End the dash if enough distance has been travelled.
    if (compteur_dash == longueur_dash) {
      is_dashing = 0;
      cooldown = 0;
    }

    // The speed increases during the dash
    if (compteur_dash == step1_dash){
      dx = dx + dx;
      dy = dy + dy;
    }
    if (compteur_dash == step2_dash){
      dx = dx + dx;
      dy = dy + dy;
    }
    if (compteur_dash == step3_dash){
      // The caracter slows down at the end of the dash

      if (dx > 30) dx = dx + 2;
      if (dx > 0 && dx < 30) dx = dx -2;
      if (dy > 30) dy = dy + 2;
      if (dy > 0 && dx < 30) dy = dy -2;
    }

    //move_sprite(PLAYER_SPRITE_ID, player.pos.x, player.pos.y);
    compteur_dash += 1;
  }
}


void handle_collisions() {
  total_diff.dx = dx;
  total_diff.dy = dy;

  while (total_diff.dx != 0 || total_diff.dy != 0) {
    // Split movement into submoves (to avoid going past obstacles)

    // XXX: the generated assembly for this line is ridiculously
    // long IMO (~104 instructions).
    current_diff.dx = ABS_CLAMP(total_diff.dx, MAX_MOVE_X);

    total_diff.dx -= current_diff.dx;
    new_player.pos.x = player.pos.x + current_diff.dx;


    // Player's position using tilemap coordinates
    effective_x = (new_player.pos.x >> 3);
    effective_y = (player.pos.y >> 3);

    for (UINT8 i = 0; i < 3; i++) {
      for (UINT8 j = 0; j < 3; j++) {
        INT8 block_x = (effective_x + i - 1);
        INT8 block_y = (effective_y + j - 1);

        if (block_x < 0 || block_x >= (INT8) ROOM_WIDTH + 2 || block_y < 0 || block_y >= (INT8) ROOM_HEIGHT + 2)  // out of bounds
          continue;

        INT16 k = block_y * (ROOM_WIDTH + 2) + block_x;

        if (TILEMAP[room_number][k] != 0) {
          block.pos.x = block_x << 3;
          block.pos.y = block_y << 3;

          if (rect_rect_collision(&new_player, &block)) {
            VEC_DIFF diff = {0, 0};
            rect_rect_penetration(&(player.pos), &(new_player.pos), &(player.size), &block, &diff);
            new_player.pos.x += diff.dx;
            total_diff.dx = 0;
          }
        }
      }
    }

    current_diff.dy = ABS_CLAMP(total_diff.dy, MAX_MOVE_Y);
    total_diff.dy -= current_diff.dy;
    new_player.pos.y = player.pos.y + current_diff.dy;

    effective_x = (new_player.pos.x >> 3);
    effective_y = (new_player.pos.y >> 3);

    for (UINT8 i = 0; i < 3; i++) {
      for (UINT8 j = 0; j < 3; j++) {
        INT8 block_x = (effective_x + i - 1);
        INT8 block_y = (effective_y + j - 1);

        if (block_x < 0 || block_x >= (INT8) ROOM_WIDTH + 2 || block_y < 0 || block_y >= (INT8) ROOM_HEIGHT + 2)  // out of bounds
          continue;

        INT16 k = block_y * (ROOM_WIDTH + 2) + block_x;

        if (TILEMAP[room_number][k] != 0) {
          block.pos.x = block_x << 3;
          block.pos.y = block_y << 3;

          if (rect_rect_collision(&new_player, &block)) {
            VEC_DIFF diff = {0, 0};
            rect_rect_penetration(&(player.pos), &(new_player.pos), &(player.size), &block, &diff);
            new_player.pos.y += diff.dy;
            total_diff.dy = 0;
          }
        }
      }
    }

    player.pos.x = new_player.pos.x;
    player.pos.y = new_player.pos.y;

    if (rect_rect_collision(&player, &left_door_hitbox)) {
      room_number = 1;
      load_tilemap();
      player.pos.x = 16 * 8;
      return;
    } else if (rect_rect_collision(&player, &right_door_hitbox)) {
      room_number = 0;
      load_tilemap();
      player.pos.x = 8;
      return;
    }

  }
}

void init_dash_state() {

    is_dashing = 0;
    compteur_dash = 0;

}

void main(void) {
    init_player_state();
    init_dash_state();

    init_graphics();

    open_doors();

    while (1) {
        // Wait for v-blank (screen refresh)
        wait_vbl_done();

        read_input();

        handle_dash();

        handle_collisions();

        // Do NOT move this near update_sprite_animation...
        move_sprite(PLAYER_SPRITE_ID, player.pos.x + SPRITE_OFFSET_X + scroll_x, player.pos.y + SPRITE_OFFSET_Y + scroll_y);

        // We do not update the animation on each frame: the animation
        // will be too quick. So we skip frames
        frame_skip -= 1;
        if (!frame_skip) {
          frame_skip = 8;
        } else {
          continue;
        }

        // XXX: this is not necessarily the right condition, but oh well.
        if (dx == 0 && dy == 0){
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
