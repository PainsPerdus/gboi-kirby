#include "room.h"

inline TILE* get_room_tiles(ROOM* room) {
    if (room->is_small)
        return room->small_tiles;
    else
        return room->big_tiles;
}