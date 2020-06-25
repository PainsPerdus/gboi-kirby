#include <gb/gb.h>

#ifndef _COLLISION_H
#define _COLLISION_H

typedef struct {
    UINT8 x;
    UINT8 y; 
    UINT8 width;
    UINT8 height;
} RECTANGLE;

/**
 * @brief Detect a collision between two rectangles
 * 
 * @param rect1 
 * @param rect2 
 * @return BOOLEAN 
 */
BOOLEAN rect_rect_collision(RECTANGLE rect1, RECTANGLE rect2);

#endif