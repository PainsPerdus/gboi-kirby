#include <gb/gb.h>
#include "collision.h"


BOOLEAN rect_rect_collision(RECTANGLE* rect1, RECTANGLE* rect2) {
    return rect1->pos.x < rect2->pos.x + rect2->size.w
    && rect1->pos.x + rect1->size.w > rect2->pos.x 
    && rect1->pos.y < rect2->pos.y + rect2->size.h
    && rect1->size.h + rect1->pos.y > rect2->pos.y;
}


