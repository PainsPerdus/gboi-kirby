#include <gb/gb.h>

#ifndef _ROOM_H
#define _ROOM_H

typedef enum { EMPTY, WALL, SPIKES, HOLE } TILE;

//// DOORS

#define DOOR_LEFT 0
#define DOOR_TOP 1
#define DOOR_RIGHT 2
#define DOOR_BOTTOM 3

typedef struct {
    BOOLEAN open;
    BOOLEAN keys[3];
    UINT8 ptr;
} DOORS;


//// ROOMS

typedef struct {
    DOORS doors[4];
    BOOLEAN is_small;
    union {
        TILE small_tiles[16*16];
        TILE big_tiles[24*24];
    };
} ROOM;

ROOM base_floor[1];

TILE* get_room_tiles(ROOM* room);


#endif