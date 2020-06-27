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
BOOLEAN rect_rect_collision(RECTANGLE* rect1, RECTANGLE* rect2);


//// COLLISION POST-PROCESSING
typedef struct {
    INT8 dx;
    INT8 dy;
} VEC_DIFF;

/**
 * @brief Compute the overlap between \p entity and \p obstacle
 * 
 * @param old_pos 
 * @param new_pos 
 * @param size Size of the entity
 * @param obstacle 
 * @param diff diff + new_pos = collision free pos
 */
void rect_rect_penetration(VEC* old_pos, VEC* new_pos, SIZE* size, RECTANGLE* obstacle, VEC_DIFF* diff);

#endif