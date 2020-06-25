#include <gb/gb.h>

typedef struct{
    UINT8 x;
    UINT8 y; 
    UINT8 width;
    UINT8 heigth;
} RECTANGLE;

//return true if there is a collision betwenn the two rectangles taken in argument
bool collision(RECTANGLE rect1, RECTANGLE rect2){
    if (rect1.x < rect2.x + rect2.width &&
        rect1.x + rect1.width > rect2.x &&
        rect1.y < rect2.y + rect2.height &&
        rect1.height + rect1.y > rect2.y) {
            return true
    }
    return false
}
