#ifndef _ENEMY_SPAWNER
#define _ENEMY_SPAWNER

#include <gb/gb.h>
#include "enemy_monster.h"
#include "global.h"


typedef struct {
    UINT8 pos;
    UINT8 id;
} ENEMY_SPAWNER;

// NOTE: WON'T WORK WITH BIG ROOMS
extern ENEMY_SPAWNER spawners[MAX_ROOM_NB][MAX_ENEMY_NB];

/**
 * @brief Add an enemy spawner at \p pos in room \p room_ptr
 * 
 * @param room_ptr 
 * @param pos 
 * @param id 
 */
void push_enemy_spawner(UINT8 room_ptr, UINT8 pos, UINT8 id);


/**
 * @brief Reset all spawner pointers
 * 
 */
void reset_enemy_spawners();

/**
 * @brief Spawn all the enemies from \p room_ptr
 * 
 * @param room_ptr 
 */
void spawn_enemies(UINT8 room_ptr);


#endif