#include <gb/gb.h>

#ifndef _GLOBAL_H
#define _GLOBAL_H

#define MAX_ROOM_NB 10

#define SMALL_ROOM_SIDE 16
#define BIG_ROOM_SIDE 24

typedef UINT8 TILE;

typedef enum {UP=0, DOWN=1, LEFT=2, RIGHT=3} DIR;

typedef struct {
    BOOLEAN is_open;
    BOOLEAN keys[3];
    UINT8 room_ptr;
} DOOR;

typedef struct {
    DOOR doors[4];
    BOOLEAN is_small;
    union  {
        TILE small_tiles[SMALL_ROOM_SIDE * SMALL_ROOM_SIDE];
        TILE big_tiles[BIG_ROOM_SIDE * BIG_ROOM_SIDE];
    };
    
} ROOM;

typedef struct {
    ROOM rooms[MAX_ROOM_NB];
    UINT8 nb_rooms;
} FLOOR;

extern FLOOR base_floor;

#endif