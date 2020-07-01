#include <gb/gb.h>
#include "enemy_spawner.h"
#include "enemy_monster.h"
#include "animations.h"


ENEMY_SPAWNER spawners[MAX_ROOM_NB][MAX_ENEMY_NB];
static UINT8 spawners_ptr[MAX_ROOM_NB];


void push_enemy_spawner(UINT8 room_ptr, UINT8 pos, UINT8 id) {
    spawners[room_ptr][spawners_ptr[room_ptr]].pos = pos;
    spawners[room_ptr][spawners_ptr[room_ptr]].id = id;
    spawners_ptr[room_ptr] ++;
}

void clear_enemy_spawners() {
    for (UINT8 i = 0; i < MAX_ROOM_NB; i++) {
        spawners_ptr[i] = 0;
    }
}

// WON'T WORK WITH BIG ROOMS
void spawn_enemies(UINT8 room_ptr) {
    for (UINT8 i = 0; i < spawners_ptr[room_ptr]; i++) {
        ENEMY_SPAWNER* spawner = &spawners[room_ptr][i];
        ENEMY* enemy = get_available_enemy();
        
        init_melee(enemy);
        enemy->xpos = (((UINT8) (spawner->pos % 16)) << 3) + 8;
        enemy->ypos = (((UINT8) (spawner->pos >> 4)) << 3) + 8;
    }
}
