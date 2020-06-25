#include <gb/gb.h>

struct Rectangle {
    UINT8 x;  // member variable
    UINT8 y; // member variable
    UINT8 width;
    UINT8 heigth;
};

bool collision(Rectangle rect1, Rectangle rect2){
    if (rect1.x < rect2.x + rect2.width &&
        rect1.x + rect1.width > rect2.x &&
        rect1.y < rect2.y + rect2.height &&
        rect1.height + rect1.y > rect2.y) {
            return true
    }
    return false
}
