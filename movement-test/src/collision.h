#include <gb/gb.h>

#ifndef _COLLISION_H
#define _COLLISION_H

typedef struct {
    UINT8 x;
    UINT8 y; 
} VEC;

typedef struct {
    UINT8 w;
    UINT8 h;
} SIZE;

typedef struct {
    VEC pos;
    SIZE size;
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