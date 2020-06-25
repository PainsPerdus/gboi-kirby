#include <gb/gb.h>
#include "collision.h"


BOOLEAN rect_rect_collision(RECTANGLE rect1, RECTANGLE rect2) {
    return rect1.x < rect2.x + rect2.width 
    && rect1.x + rect1.width > rect2.x 
    && rect1.y < rect2.y + rect2.height 
    && rect1.height + rect1.y > rect2.y;
}


