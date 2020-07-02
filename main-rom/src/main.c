#include <stdio.h>
#include <gb/gb.h>

#include <rand.h>

#include "enemy_monster.h"
#include "dummy.sprites.h"
#include "player.sprites.h"
#include "collision.h"
#include "tileset.h"
#include "ui_tileset.h"
#include "sound_effect.h"
#include "global.h"
#include <string.h>
#include "floorgen.h"
#include "chainsaw_lateral.sprites.h"
#include "chainsaw_vertical.sprites.h"
#include "animations.h"
#include "oam_pool.h"

// Variables containing player state
static RECTANGLE player;
static RECTANGLE new_player;
static UINT8 player_direction;
static UINT8 player_animation_frame;

#define SPRITE_OFFSET_X 8
#define SPRITE_OFFSET_Y 8  // note: set at 8 even though sprites are 8px tall because of the perspective

#define TILE_SIZE_PX 8
#define MAX_MOVE_X (TILE_SIZE_PX - 1)
#define MAX_MOVE_Y (TILE_SIZE_PX - 1)


const UINT8 ROOM_WIDTH = 16;  // note: should fetch that from the room's data
const UINT8 ROOM_HEIGHT = 16;

INT8 scroll_x = 0;  // initial offset because of the UI
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


void flip_sprite_vert(UINT8 sprite_id) {
    set_sprite_prop(sprite_id, get_sprite_prop(sprite_id) | S_FLIPY);
}

void unflip_sprite_vert(UINT8 sprite_id) {
    set_sprite_prop(sprite_id, get_sprite_prop(sprite_id) & ~S_FLIPY);
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

UINT8 TILEMAP[18*18] = {
   4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 5,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
  11, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 9,
   7,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10,10, 6
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

static UINT8 dash_cooldown = 30; // in frames
static UINT8 longueur_dash;
static UINT8 step1_dash;
static UINT8 step2_dash;
static UINT8 step3_dash;
static UINT8 compteur_dash;
static UINT8 dash_frame_counter = 255;

static BOOLEAN is_dashing;

static RECTANGLE door_hitboxes[4];

static UINT8 chainsaw_frame_counter = 150;

static UINT8 chainsaw_cooldown = 100;

static UINT8 chainsaw_relativ_x;
static UINT8 chainsaw_relativ_y;
static UINT8 chainsaw_animation_length = 20;
static UINT8 chainsaw_animation_part1 = 7;
static UINT8 chainsaw_animation_part2 = 14;
static UINT8 offset_chainsaw = 0;                 //for the 2 sprites high image, we have to move one part from the other, depending of the orientation

static UINT8 room_number = 0;

FLOOR base_floor;


static UINT8 invincibility_time = 0;


void player_hit() {
  if (invincibility_time > 0)
    return;
  invincibility_time = 60;
}


/**
 * @brief Replace the background layer with TILEMAP
 * WON't WORK FOR BIG ROOMS
 */
void load_tilemap() {
    set_bkg_tiles(0, 0, 18, 18, TILEMAP);
}

static const UINT16 door_positions_small[8] = {8, 9, 17*18+8, 17*18+9, 8*18, 9*18, 9*18-1, 10*18-1};
static const UINT16 door_positions_big[8] = {13, 14, 25*26+13, 25*26+14, 13*26, 14*26, 14*26-1, 15*26-1};

static const UINT8 open_door_tiles[2] = {0, 0};
static const UINT8 close_door_tiles[2] = {1, 1};

/**
 * @brief Replace door tiles
 *
 * Call this when doors have been opened / closed
 */
void reset_doors() {
  ROOM* room = &base_floor.rooms[room_number];
  for (UINT8 i = 0; i < 4; i++) {
    BOOLEAN open = (room->doors[i].is_open);
    UINT16 door_pos1 = room->is_small ? door_positions_small[(i << 1)] : door_positions_big[(i << 1)];
    UINT16 door_pos2 = room->is_small ? door_positions_small[(i << 1) + 1] : door_positions_big[(i << 1) + 1];
    TILEMAP[door_pos1] = open ? 0 : 1;
    TILEMAP[door_pos2] = open ? 0 : 1;
    UINT8 size = (room->is_small ? SMALL_ROOM_SIDE : BIG_ROOM_SIDE) + 2; // +2 because SMALL_ROOM_SIDE
                                                                         // and BIG_ROOM_SIDE don't take
                                                                         // the outer walls into account

    UINT8 x = (door_pos1 % size);
    UINT8 y = door_pos1 / size;
    UINT8 w = (i < 2) ? 2 : 1;  // i < 2 <=> i = UP or DOWN
    UINT8 h = (i < 2) ? 1 : 2;

    door_hitboxes[i].pos.x = x << 3;
    door_hitboxes[i].pos.y = y << 3;
    door_hitboxes[i].size.w = w << 3;
    door_hitboxes[i].size.h = h << 3;

    set_bkg_tiles(x, y, w, h, open ? open_door_tiles : close_door_tiles);
  }
}

static void open_doors() {
  ROOM* room = &base_floor.rooms[room_number];
  for (UINT8 i = 0; i < 4; i++) {
    if (room->doors[i].keys[0] + room->doors[i].keys[1] + room->doors[i].keys[2] < 3) {
      room->doors[i].is_open = TRUE;
    }
  }
}


/**
 * @brief Replace the currently loaded room in TILEMAP with base_floor.rooms[room_number]
 */
void load_room() {
  UINT8 size = base_floor.rooms[room_number].is_small ? SMALL_ROOM_SIDE : BIG_ROOM_SIDE;
  for (UINT8 i = 0; i < size; i++) {
    TILE* tilemap_ptr = TILEMAP + ((size+2) + 1 + i*(size+2));
    TILE* room_ptr = base_floor.rooms[room_number].small_tiles + (i * size);
    memcpy(tilemap_ptr, room_ptr, size);
  }
}

void load_enemies() {
  for (UINT8 i = 0; i < enemy_stack_ptr; i++) 
    enemy_release(&enemy_stack[i]);
    enemy_stack_ptr = 0;
    spawn_enemies(room_number);

  for (UINT8 i = 0; i < enemy_stack_ptr; i++) {
    display_enemy(&enemy_stack[i], enemy_stack[i].xpos, enemy_stack[i].ypos);
  }
}

/**
 * @brief Check whether the player should leave the room or not
 */
void check_doors() {
  ROOM* room = &base_floor.rooms[room_number];

  for (UINT8 i = 0; i < 4; i++) {
    if (rect_rect_collision(&player, &door_hitboxes[i])) {
      room_number = room->doors[i].room_ptr;
      room = &base_floor.rooms[room_number];
      load_room();
      load_tilemap();

      // if doors are open, then that means that all enemies are dead already
      if (!(room->doors[0].is_open || room->doors[1].is_open || room->doors[2].is_open || room->doors[3].is_open))
        load_enemies();
      reset_doors();
      
      room = &base_floor.rooms[room_number];
      UINT8 size = room->is_small ? SMALL_ROOM_SIDE : BIG_ROOM_SIDE;

      switch (i) {
        case LEFT:
          player.pos.x = (size - 2) << 3;
          player.pos.y = size << 2;
          break;

        case RIGHT:
          player.pos.x = 8;
          player.pos.y = size << 2;
          break;

        case UP:
          player.pos.x = size << 2;
          player.pos.y = (size - 2) << 3;
          break;

        case DOWN:
          player.pos.x = size << 2;
          player.pos.y = 8;
          break;
      }
      return;
    }
  }
}


void add_heart();
void remove_heart();


void read_input() {
    // Read joypad keys to know if the player is walking
    // and in which direction
    keys = joypad();
    if (!is_dashing) {
      if (dash_frame_counter < 250){
          dash_frame_counter++;
      }

      dx = 0;
      dy = 0;
      if (keys & J_UP) {
          player_direction = PLAYER_DIRECTION_UP;
          dy -= 1;
          // add_heart();
	      // play_falling_sound();
      } if (keys & J_DOWN) {
          player_direction = PLAYER_DIRECTION_DOWN;
          dy += 1;
          // remove_heart();
	      // play_hit_sound();
      } if (keys & J_LEFT) {
          player_direction = PLAYER_DIRECTION_LEFT;
          dx -= 1;
          // remove_crystal();
	      // play_dash_sound();
      } if (keys & J_RIGHT) {
          player_direction = PLAYER_DIRECTION_RIGHT;
          dx += 1;
          // add_crystal();
	      // play_chainsaw_attack_sound();
      }

      if(keys & J_B){
        if (dash_frame_counter > dash_cooldown){
          player_direction += 36;             //dash tiles = direction tile + 36
          is_dashing = 1;
          compteur_dash = 0;


          dx = dx + dx;
          dy = dy + dy;
        }
      }

    }
    if(keys & J_A){
        if (chainsaw_frame_counter > chainsaw_cooldown){
          chainsaw_frame_counter = 0;
        }
      }

}



// XXX: this should be moved elsewhere.

#define WINDOW_TILEMAP_WIDTH 2
#define WINDOW_TILEMAP_HEIGHT 18



UINT8 WINDOW_TILEMAP[] = {
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
    0x00, 0x00,
};


void init_sprites() {
    // Load sprites' tiles in video memory
    set_sprite_data(FIRST_PLAYER_SPRITE, PLAYER_SPRITES_TILE_COUNT, PLAYER_SPRITES);
    set_sprite_data(FIRST_CHAINSAW_LATERAL_SPRITE, CHAINSAW_LATERAL_SPRITES_TILE_COUNT, CHAINSAW_LATERAL_SPRITES);
    set_sprite_data(FIRST_CHAINSAW_VERTICAL_SPRITE, CHAINSAW_VERTICAL_SPRITES_TILE_COUNT, CHAINSAW_VERTICAL_SPRITES);
    set_sprite_data(FIRST_DUMMY_SPRITE, DUMMY_SPRITES_TILE_COUNT, DUMMY_SPRITES);

    // https://gbdev.gg8.se/wiki/articles/GBDK_set_sprite_prop
    set_sprite_prop(PLAYER_SPRITE_ID, 0x00U);


    // XXX: not sure this should be here.
    // Init the sprite used for the player
    // We have to add an offset because of the way the gb handles sprites
    move_sprite(PLAYER_SPRITE_ID, player.pos.x + SPRITE_OFFSET_X + scroll_x, player.pos.y + SPRITE_OFFSET_Y + scroll_y);

    // Use 8x16 sprites
    SPRITES_8x16;
    // Makes sprites "layer" visible
    SHOW_SPRITES;
}

// 144 / 8 = 18 = max we can display given 8x8 tiles.
#define MAX_HEARTS WINDOW_TILEMAP_HEIGHT
#define MAX_CRYSTALS WINDOW_TILEMAP_HEIGHT

static UINT8 heart_count = 0;
static UINT8 crystal_count = 0;


// XXX: hardcoded... should be relative.
#define HEART_TILE 0x14
#define CRYSTAL_TILE 0x15
#define EMPTY_HEART_TILE 0x16
#define EMPTY_CRYSTAL_TILE 0x17

void load_win_tilemap();



void set_hearts(UINT8 x) {
  heart_count = x;

  int i = 0;

  // Empty tiles.
  while (i < (MAX_HEARTS - heart_count)) {
    WINDOW_TILEMAP[(i << 1)] = EMPTY_HEART_TILE;
    i++;
  }


  // Heart tiles.
  while (i < MAX_HEARTS) {
    WINDOW_TILEMAP[(i << 1)] = HEART_TILE;
    i++;
  }

  load_win_tilemap();
}

void set_crystals(UINT8 x) {
  crystal_count = x;

  int i = 0;

  // Empty tiles.
  while (i < (MAX_HEARTS - crystal_count)) {
    WINDOW_TILEMAP[(i << 1) + 1] = EMPTY_CRYSTAL_TILE;
    i++;
  }


  // Heart tiles.
  while (i < MAX_HEARTS) {
    WINDOW_TILEMAP[(i << 1) + 1] = CRYSTAL_TILE;
    i++;
  }

  load_win_tilemap();
}





void add_heart() {
  if (heart_count < MAX_HEARTS) set_hearts(heart_count + 1);
}


// XXX: use this function for death?
void remove_heart() {
  if (heart_count > 0) set_hearts(heart_count - 1);
}


void add_crystal() {
  if (crystal_count < MAX_CRYSTALS) set_crystals(crystal_count + 1);
}


void remove_crystal() {
  if (crystal_count > 0) set_crystals(crystal_count - 1);
}




void load_win_tilemap() {
  set_win_tiles(0, 0, WINDOW_TILEMAP_WIDTH, WINDOW_TILEMAP_HEIGHT, WINDOW_TILEMAP);
}


void init_background_and_window() {
    // For the actual background
    set_bkg_data(0, TILESET_TILE_COUNT, TILESET);
    load_tilemap();
    SHOW_BKG;


    // For the window. Its tiles come immediately after the background tiles.
    set_bkg_data(TILESET_TILE_COUNT, UI_TILESET_TILE_COUNT, UI_TILESET);
    load_win_tilemap();

    // Top-left corner is at (7, 0).
    // The Game Boy screen is 160 pixels wide
    // The UI bar is 16 px wide
    // TODO: move this into #define.
    move_win(7 + 160 - 16, 0);

    SHOW_WIN;
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
  // If the player is dashing, he can't control his movement anymore
  // after x iterations of the dashing code, we set is_dashing to 0 and that ends the dash
  if (is_dashing) {
    longueur_dash = 20;
    step1_dash = 5;
    step2_dash = 10;
    step3_dash = 15;

    // The length of the dash is reduced if the movement is diagonal.
    if (dx != 0 && dy != 0){
      longueur_dash = 18;
      step1_dash = 4;
      step2_dash = 9;
      step3_dash = 15;
    }

    // End the dash if enough distance has been travelled.
    if (compteur_dash == longueur_dash) {
      is_dashing = 0;
      dash_frame_counter = 0;
    }

    // The speed increases during the dash
    if (compteur_dash == step1_dash){
      dx = dx + dx;
      dy = dy + dy;
    }
    if (compteur_dash == step2_dash){
      //dx = dx + dx;
      //dy = dy + dy;
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

void handle_chainsaw() {
  reset_chainsaw();

  // Guarding against overflows.
  if (chainsaw_frame_counter < 255 ){
    chainsaw_frame_counter += 1;
  }

  if (chainsaw_frame_counter < chainsaw_animation_part1) {
    if (player_direction == PLAYER_DIRECTION_LEFT || player_direction == PLAYER_DASH_DIRECTION_LEFT) {
      chainsaw_relativ_x = 10;
      chainsaw_relativ_y = 5;
      flip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_TOP_LATERAL_SPRITE_ID);
    } else if (player_direction == PLAYER_DIRECTION_RIGHT || player_direction == PLAYER_DASH_DIRECTION_RIGHT) {
      chainsaw_relativ_x = 250;
      chainsaw_relativ_y = 5;
      unflip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_TOP_LATERAL_SPRITE_ID);
    } else if (player_direction == PLAYER_DIRECTION_UP || player_direction == PLAYER_DASH_DIRECTION_UP) {
      chainsaw_relativ_x = 251;
      chainsaw_relativ_y = 263;
      flip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_VERTICAL_SPRITE_ID);
    } else if (player_direction == PLAYER_DIRECTION_DOWN || player_direction == PLAYER_DASH_DIRECTION_DOWN) {
      chainsaw_relativ_x = 8;
      chainsaw_relativ_y = 253;
      unflip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_VERTICAL_SPRITE_ID);
    }
  }


  // Wooosh
  if (chainsaw_frame_counter > chainsaw_animation_part1) {
    if (player_direction == PLAYER_DIRECTION_LEFT || player_direction == PLAYER_DASH_DIRECTION_LEFT) {
      chainsaw_relativ_x = 248;
      chainsaw_relativ_y = 252;
      flip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      flip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID +1);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_TOP_LATERAL_SPRITE_ID + 4);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 1, CHAINSAW_TOP_LATERAL_SPRITE_ID + 2);
    }
    if (player_direction == PLAYER_DIRECTION_RIGHT || player_direction == PLAYER_DASH_DIRECTION_RIGHT) {
      chainsaw_relativ_x = 0;
      chainsaw_relativ_y = 250 ;
      unflip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      unflip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID +1);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_TOP_LATERAL_SPRITE_ID + 2);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 1, CHAINSAW_TOP_LATERAL_SPRITE_ID + 4);
    }

    if (player_direction == PLAYER_DIRECTION_UP || player_direction == PLAYER_DASH_DIRECTION_UP) {
      chainsaw_relativ_x = 249;
      chainsaw_relativ_y = 247;
      flip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      flip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_VERTICAL_SPRITE_ID + 4);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2, CHAINSAW_VERTICAL_SPRITE_ID + 2);
      offset_chainsaw = 0;
    }
    if (player_direction == PLAYER_DIRECTION_DOWN || player_direction == PLAYER_DASH_DIRECTION_DOWN) {
      chainsaw_relativ_x = 8;
      chainsaw_relativ_y = 253;
      unflip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      unflip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_VERTICAL_SPRITE_ID + 2);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2, CHAINSAW_VERTICAL_SPRITE_ID + 4);
      offset_chainsaw = 2;
    }


  }

  // Chainsaw hit
  if (chainsaw_frame_counter > chainsaw_animation_part2){
    // "put back blank in second sprite of the woosh"
    set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2, CHAINSAW_TOP_LATERAL_SPRITE_ID - 2);

    if (player_direction == PLAYER_DIRECTION_LEFT || player_direction == PLAYER_DASH_DIRECTION_LEFT) {
      chainsaw_relativ_x = 241;
      chainsaw_relativ_y = 3;
      flip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_TOP_LATERAL_SPRITE_ID + 8);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 1, CHAINSAW_TOP_LATERAL_SPRITE_ID + 6);
    } else if (player_direction == PLAYER_DIRECTION_RIGHT || player_direction == PLAYER_DASH_DIRECTION_RIGHT) {
      chainsaw_relativ_x = 7;
      chainsaw_relativ_y = 3;
      unflip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_TOP_LATERAL_SPRITE_ID + 6);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 1, CHAINSAW_TOP_LATERAL_SPRITE_ID + 8);
    } else if (player_direction == PLAYER_DIRECTION_UP || player_direction == PLAYER_DASH_DIRECTION_UP) {
      chainsaw_relativ_x = 249;
      chainsaw_relativ_y = 252;
      flip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_VERTICAL_SPRITE_ID +6);
    } else if (player_direction == PLAYER_DIRECTION_DOWN || player_direction == PLAYER_DASH_DIRECTION_DOWN) {
      chainsaw_relativ_x = 8;
      chainsaw_relativ_y = 10;
      unflip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID);
      set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_VERTICAL_SPRITE_ID + 6);
    }

  }

  if (chainsaw_frame_counter > chainsaw_animation_length) {
    reset_chainsaw();
  }

}


// reset all sprites id   (to fix a bunch of bugs)
void reset_chainsaw(){
  unflip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID);
  unflip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID + 1);
  unflip_sprite_vert(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2);
  unflip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID);
  unflip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID + 1);
  unflip_sprite_horiz(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2);
  set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID, CHAINSAW_TOP_LATERAL_SPRITE_ID - 2);
  set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 1, CHAINSAW_TOP_LATERAL_SPRITE_ID - 2);
  set_sprite_tile(CHAINSAW_TOP_LATERAL_SPRITE_ID + 2, CHAINSAW_TOP_LATERAL_SPRITE_ID - 2);
}


void collision_check(BOOLEAN x_move) {
  VEC_DIFF diff = {0, 0};

  for (UINT8 i = 0; i < 3; i++) {
      for (UINT8 j = 0; j < 3; j++) {
        INT8 block_x = (effective_x + i - 1);
        INT8 block_y = (effective_y + j - 1);

        if (block_x < 0 || block_x >= (INT8) ROOM_WIDTH + 2 || block_y < 0 || block_y >= (INT8) ROOM_HEIGHT + 2)  // out of bounds
          continue;

        INT16 k = block_y * (ROOM_WIDTH + 2) + block_x;

        // check collision if tile is not empty or tile is a hole but Pinney isn't invincible and dashing
        if (TILEMAP[k] != 0 && !((is_dashing && invincibility_time == 0) && TILEMAP[k] == 3)) {
          block.pos.x = block_x << 3;
          block.pos.y = block_y << 3;

          if (rect_rect_collision(&new_player, &block)) {
            rect_rect_penetration(&(player.pos), &(new_player.pos), &(player.size), &block, &diff);
            
            if (x_move){
            new_player.pos.x += diff.dx;
            total_diff.dx = 0;
            } else {
              new_player.pos.y += diff.dy;
              total_diff.dy = 0;
            }

            // spikes
            if (TILEMAP[k] == 2) {
              player_hit();
            }
          }

        }
      }
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

    collision_check(TRUE);

    current_diff.dy = ABS_CLAMP(total_diff.dy, MAX_MOVE_Y);
    total_diff.dy -= current_diff.dy;
    new_player.pos.y = player.pos.y + current_diff.dy;

    effective_x = (new_player.pos.x >> 3);
    effective_y = (new_player.pos.y >> 3);

    collision_check(FALSE);

    player.pos.x = new_player.pos.x;
    player.pos.y = new_player.pos.y;

    check_doors();
  }
}

void init_dash_state() {
    is_dashing = 0;
    compteur_dash = 0;

}

void init_chainsaw_state() {
    is_dashing = 0;
    compteur_dash = 0;
}

UINT8 last_safe_x;
UINT8 last_safe_y;

// won't work with big rooms
void handle_fall() {
  UINT8 x = (player.pos.x + 4) >> 3;
  UINT8 y = (player.pos.y + 4) >> 3;

  if (TILEMAP[y * (SMALL_ROOM_SIDE + 2) + x] == 3) {
    if (!is_dashing) {
      player_hit();
      player.pos.x = last_safe_x << 3;
      player.pos.y = last_safe_y << 3;
      play_falling_sound();
    }
  } else {
    last_safe_x = x;
    last_safe_y = y;
  }
}


void start_screen() {
    printf("Press START!\n");

    UINT16 frames_elapsed = 0;
    while (!(joypad() & J_START)) {
      frames_elapsed++;
    }

    // Use the human component to initialize RNG :)
    initarand(frames_elapsed);
}




void main(void) {
    start_screen();

    borrow_oam_id();  // player
    borrow_oam_id();  // chainsaw1
    borrow_oam_id();  // chainsaw2
    //
    // XXX.
    set_hearts(0);
    set_crystals(0);

    gen_floor();

    load_room();
    reset_doors();

    load_enemies();

    init_player_state();
    init_dash_state();
    init_chainsaw_state();

    init_sprites();
    init_background_and_window();

    while (1) {
      // Wait for v-blank (screen refresh)
      wait_vbl_done();

      read_input();

      handle_dash();

      handle_chainsaw();

      handle_collisions();
      handle_fall();

      // Do NOT move this near update_sprite_animation...
      move_sprite(PLAYER_SPRITE_ID, player.pos.x + SPRITE_OFFSET_X + scroll_x, player.pos.y + SPRITE_OFFSET_Y + scroll_y);

      // Some saw assets are 16x16 and therefore require two side-by-side 8x16 sprites.
      move_sprite(CHAINSAW_TOP_LATERAL_SPRITE_ID, player.pos.x + SPRITE_OFFSET_X + scroll_x + chainsaw_relativ_x, player.pos.y + SPRITE_OFFSET_Y + scroll_y + chainsaw_relativ_y);
      move_sprite(CHAINSAW_TOP_LATERAL_SPRITE_ID+1, player.pos.x + SPRITE_OFFSET_X + scroll_x + chainsaw_relativ_x + 8, player.pos.y + SPRITE_OFFSET_Y + scroll_y + chainsaw_relativ_y);      //for the 16*16 chainsaw animation
      move_sprite(CHAINSAW_TOP_LATERAL_SPRITE_ID+2,offset_chainsaw + player.pos.x + SPRITE_OFFSET_X + scroll_x + chainsaw_relativ_x,16+ player.pos.y + SPRITE_OFFSET_Y + scroll_y + chainsaw_relativ_y);      //for the 16*16 sainwhaw animation
      
      BOOLEAN all_dead = TRUE;
      for (UINT8 i = 0; i < enemy_stack_ptr; i++) {
        if (enemy_stack[i].health > 0) {
          handle_enemy_attack(&enemy_stack[i]);
          all_dead = FALSE;
        } else 
          enemy_death(&enemy_stack[i]);
      }

      // player invincibility graphics
      if (invincibility_time > 0) {
        if ((50 < invincibility_time && invincibility_time <= 60) || (30 < invincibility_time && invincibility_time <= 40) || (10 < invincibility_time && invincibility_time <= 20)) {
          move_sprite(PLAYER_SPRITE_ID, X_OFFSCREEN, Y_OFFSCREEN);
          move_sprite(CHAINSAW_TOP_LATERAL_SPRITE_ID, X_OFFSCREEN, Y_OFFSCREEN);
          move_sprite(CHAINSAW_TOP_LATERAL_SPRITE_ID+1, X_OFFSCREEN, Y_OFFSCREEN);      //for the 16*16 chainsaw animation
        }
        if (invincibility_time == 60) {
          wait_vbl_done();
          wait_vbl_done();
        }
        invincibility_time --;
      }


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

      if (all_dead) {
        open_doors();
        reset_doors();
      }


          
      // SECTION HANDLING ENEMIES
      // If there are several enemies, the following is to be done with EACH enemy that is alive.
      /*if (basic.health > 0) { // Enemy is alive: handle its walk and its attack
        handle_enemy_walk(&basic);
        handle_enemy_attack(&basic);
      } else {
        // Enemy about to die: play death animation.
        enemy_death(&basic);
      }*/
    }
}
