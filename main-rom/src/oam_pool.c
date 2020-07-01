#include "oam_pool.h"

UINT8 oam_stack[40] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39};
UINT8 oam_stack_ptr = 0;

UINT8 borrow_oam_id() {
    UINT8 id = oam_stack[oam_stack_ptr];
    oam_stack_ptr ++;
    return id;
}

void free_oam_id(UINT8 id) {
    oam_stack_ptr --;
    oam_stack[oam_stack_ptr] = id;
}