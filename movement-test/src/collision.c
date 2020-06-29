#include <gb/gb.h>
#include "collision.h"


BOOLEAN rect_rect_collision(RECTANGLE* rect1, RECTANGLE* rect2) {
    return rect1->pos.x < rect2->pos.x + rect2->size.w
    && rect1->pos.x + rect1->size.w > rect2->pos.x
    && rect1->pos.y < rect2->pos.y + rect2->size.h
    && rect1->size.h + rect1->pos.y > rect2->pos.y;
}

void rect_rect_penetration(VEC* old_pos, VEC* new_pos, SIZE* size, RECTANGLE* obstacle, VEC_DIFF* diff) {
    diff->dx = 0;
    diff->dy = 0;

    // player from left
    if (old_pos->x + size->w <= obstacle->pos.x
        && obstacle->pos.x < new_pos->x + size->w)
        diff->dx = obstacle->pos.x - (new_pos->x + size->w);

    // player from top
    if (old_pos->y + size->h <= obstacle->pos.y
        && obstacle->pos.y < new_pos->y + size->h)
        diff->dy = obstacle->pos.y - (new_pos->y + size->h);

    // player from right
    if (old_pos->x >= obstacle->pos.x + obstacle->size.w
        && obstacle->pos.x + obstacle->size.w >= new_pos->x)
        diff->dx = obstacle->pos.x + obstacle->size.w - new_pos->x;

    // player from bottom
    if (old_pos->y >= obstacle->pos.y + obstacle->size.h
        && obstacle->pos.y + obstacle->size.h >= new_pos->y)
        diff->dy = obstacle->pos.y + obstacle->size.h - new_pos->y;


}
