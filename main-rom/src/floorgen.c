#include <gb/gb.h>
#include <rand.h>
#include "floorgen.h"
#include "global.h"
#include "room_gen.h"

/**
 * @brief Describe all possible room operations
 * 
 * - NOP = no operation
 * - ADD_ROOM = add a common room
 * - ADD_BOSS = add a boss room
 */
typedef enum {NOP, ADD_ROOM, ADD_BOSS} OPERATIONS;

static const DIR INV_DIR_TABLE[4] = {DOWN, UP, RIGHT, LEFT};
#define INV_DIR(x) INV_DIR_TABLE[(x)]

#define urand() ((UINT8) rand())

typedef struct {
    BOOLEAN is_null;
    UINT8 ptr;
} ROOM_POINTER;

// NOTE: could be compressed (pos -> 7 bits, is_boss -> 1 bit)
typedef struct BUILD_ROOM {
    UINT8 address;
    UINT8 pos;
    BOOLEAN is_boss;
    ROOM_POINTER neighbours_ptr[4];
} BUILD_ROOM;


#define NB_NEIGHBOURS(x) (!(x).neighbours_ptr[UP].is_null + !(x).neighbours_ptr[DOWN].is_null + !(x).neighbours_ptr[LEFT].is_null +  !(x).neighbours_ptr[RIGHT].is_null)
#define COORD(x,y) ((y) << FLOOR_SIDE_LOG) + (x)
#define POS_VALID(x) (x) < FLOOR_SIDE_SQ

static BOOLEAN ROOM_EXPANDABLE(BUILD_ROOM* room) {
    return NB_NEIGHBOURS(*room) < 4;
}

#define SET_NEIGHBOUR(room,dir,neigh_adress) (room).neighbours_ptr[(dir)].is_null = FALSE; (room).neighbours_ptr[(dir)].ptr = (neigh_adress);

static UINT8 nb_rooms = 0;
#define MIN_ROOMS 3
#define MAX_ROOMS 10

#define FLOOR_SIDE 8
#define FLOOR_SIDE_LOG 3  // 2^FLOOR_SIDE_LOG = FLOOR_SIDE
#define FLOOR_SIDE_SQ FLOOR_SIDE * FLOOR_SIDE


static OPERATIONS build_list[MAX_ROOMS] = {NOP};
static UINT8 build_list_ptr = 0;

/**
 * @brief Room reserve and stack
 * 
 * Contains all available rooms and rooms whose indices are
 * between 0 (inclusive) and build_list_ptr (exclusive) have
 * already been added to the current floor
 */
static BUILD_ROOM built[MAX_ROOMS];

/**
 * @brief Grid representing the floor
 * 
 */
static ROOM_POINTER floor[FLOOR_SIDE_SQ];

/**
 * @brief Completely reset the room built[index]
 * 
 * @param index 
 */
static void reset_room(UINT8 index) {
    floor[built[index].pos].is_null = TRUE;
    built[index].is_boss = FALSE;
    built[index].address = index;
    built[index].pos = 0;
    built[index].neighbours_ptr[0].is_null = TRUE;
    built[index].neighbours_ptr[1].is_null = TRUE;
    built[index].neighbours_ptr[2].is_null = TRUE;
    built[index].neighbours_ptr[3].is_null = TRUE;
}

/**
 * @brief Flag the latest built room as not built and reset it
 */
static void pop_room() {
    build_list_ptr --;
    reset_room(build_list_ptr);
}

/**
 * @brief Flag the next free room as added to the floor
 */
static void push_room() {
    BUILD_ROOM* room = &built[build_list_ptr];
    floor[room->pos].ptr = room->address;
    floor[room->pos].is_null = FALSE;
    build_list_ptr ++;
}

/**
 * @brief Indicates whether some operation had errors
 */
static BOOLEAN error_flag = FALSE;

/**
 * @brief Find one random expandable room. Sets error_flag as TRUE if
 * no available room is found.
 * 
 * @return UINT8 The index of the room
 */
static UINT8 get_expandable_room() {
    error_flag = FALSE;
    UINT8 index = urand() % build_list_ptr;
    UINT8 nb = 1;
    while (!ROOM_EXPANDABLE(&built[index]) && nb < build_list_ptr) {
        index++;
        nb++;
        if (nb == build_list_ptr) {
            error_flag = TRUE;
            return 0;
        }
    }
    return built[index].address;
}

/**
 * @brief Find a random direction in which room can be expanded
 * 
 * @param room Room to be expanded
 * @return DIR The direction in which \p room can be extended
 */
static DIR get_available_expansion_dir(BUILD_ROOM* room) {
    error_flag = FALSE;
    UINT8 x = room->pos % FLOOR_SIDE;
    UINT8 y = room->pos >> 3;
    DIR available[4] = {0};
    UINT8 nb_available = 0;
    if (x > 0 && floor[COORD(x - 1, y)].is_null) {
        available[nb_available] = LEFT;
        nb_available ++;
    }
    if (x < FLOOR_SIDE - 1 && floor[COORD(x + 1, y)].is_null) {
        available[nb_available] = RIGHT;
        nb_available ++;
    }
    if (y > 0 && floor[COORD(x, y - 1)].is_null) {
        available[nb_available] = UP;
        nb_available ++;
    }
    if (y < FLOOR_SIDE - 1 && floor[COORD(x, y + 1)].is_null) {
        available[nb_available] = DOWN;
        nb_available ++;
    }

    if (nb_available == 0)
        error_flag = TRUE;

    return available[urand() % nb_available];
}


/**
 * @brief Offset \p pos by \p dir
 * 
 * @param dir 
 * @param pos 
 * @return UINT8 The offset position
 */
static UINT8 add_dir_to_pos(DIR dir, INT8 pos) {
    error_flag = FALSE;
    if (dir == UP && pos > FLOOR_SIDE) {
        return pos - FLOOR_SIDE;
    } else if (dir == DOWN && pos < FLOOR_SIDE_SQ - FLOOR_SIDE) {
        return pos + FLOOR_SIDE;
    } else if (dir == LEFT && (pos % 8) > 0) {
        return pos - 1;
    } else if (dir == RIGHT && (pos % 8) < FLOOR_SIDE - 1) {
        return pos + 1;
    }
    error_flag = TRUE;
    return 0;
}


/**
 * @brief Reset everything and begin another room generation
 */
static void init_floorgen() {
    // reset everything
    build_list_ptr = 0;

    for (UINT8 i = 0; i < FLOOR_SIDE_SQ; i++)
        floor[i].is_null = TRUE;

    for (UINT8 i = 0; i < MAX_ROOMS; i++)
        reset_room(i);

    // prepare next floor generation
    nb_rooms = MIN_ROOMS + (urand() % (MAX_ROOMS + 1 - MIN_ROOMS));
    for (UINT8 i = 0; i < MAX_ROOMS; i++) {
        if (i < nb_rooms - 1)
            build_list[i] = ADD_ROOM;
        else if (i == nb_rooms - 1)
            build_list[i] = ADD_BOSS;
        else
            build_list[i] = NOP;
    }

    // add first room
    BUILD_ROOM* root = &built[0];
    root->is_boss = FALSE;
    root->pos = COORD(FLOOR_SIDE >> 1, FLOOR_SIDE >> 1);
    push_room();
}

void gen_floor() {
    // reset everything and add the root room
    init_floorgen();

    // build rooms
    while (build_list_ptr < nb_rooms) {
        // Step 1: find an expandable room
        UINT8 room_ptr = get_expandable_room();
        if (error_flag) {  // no available room: cancel last move
            pop_room();
            continue;
        }
        BUILD_ROOM* room = &built[room_ptr];
        UINT8 pos = room->pos;
        
        // Step 2: expand it. It should be possible, 
        // given how `get_expandable_room()` works
        DIR dir = get_available_expansion_dir(room);
        if (error_flag) {  // no available expansion (shouldn't happen)
            pop_room();
            continue;
        }

        // Process the newest room
        UINT8 new_pos = add_dir_to_pos(dir, pos);
        BUILD_ROOM* new_room = &built[build_list_ptr];
        new_room->pos = new_pos;
        room->neighbours_ptr[dir].ptr = new_room->address;
        SET_NEIGHBOUR(*room, dir, new_room->address);
        SET_NEIGHBOUR(*new_room, INV_DIR(dir), room->address);

        if (build_list[build_list_ptr] == ADD_ROOM) {
            // Link the newest room with its neighbours
            // Could be probabilistic instead (just make sure not
            // to unlink it from its parent)
            for (UINT8 i = 0; i < 4; i++) {
                UINT8 test_pos = add_dir_to_pos(i, new_pos);
                if (error_flag)
                    continue;

                ROOM_POINTER* test_room_ptr = &floor[test_pos];
                if (test_room_ptr->is_null)
                    continue;

                BUILD_ROOM* test_room = &built[test_room_ptr->ptr];
                SET_NEIGHBOUR(*new_room, i, test_room->address)
                SET_NEIGHBOUR(*test_room, INV_DIR(i), new_room->address);
            }
        } else if (build_list[build_list_ptr] == ADD_BOSS) {
            new_room->is_boss = TRUE;
        }

        push_room();
    }

    // convert graph to rooms
    base_floor.nb_rooms = build_list_ptr;
    for (UINT8 i = 0; i < build_list_ptr; i++) {
        base_floor.rooms[i].is_small = TRUE;
        for (UINT8 j = 0; j < 4; j++) {
            if (built[i].neighbours_ptr[j].is_null) {
                base_floor.rooms[i].doors[j].keys[0] = 1;
                base_floor.rooms[i].doors[j].keys[1] = 1;
                base_floor.rooms[i].doors[j].keys[2] = 1;
                base_floor.rooms[i].doors[j].room_ptr = 0;
                base_floor.rooms[i].doors[j].is_open = FALSE;
            } else {
                base_floor.rooms[i].doors[j].keys[0] = 0;
                base_floor.rooms[i].doors[j].keys[1] = 0;
                base_floor.rooms[i].doors[j].keys[2] = 0;
                base_floor.rooms[i].doors[j].room_ptr = built[i].neighbours_ptr[j].ptr;
                base_floor.rooms[i].doors[j].is_open = FALSE;
            }
        }
        gen_room(&base_floor.rooms[i], i);
    }

}