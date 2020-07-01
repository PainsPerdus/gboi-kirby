#ifndef _OAM_STACK
#define _OAM_STACK
#include <gb/gb.h>

extern UINT8 oam_stack[40];
extern UINT8 oam_stack_ptr;

/**
 * @brief Get an available OAM id. If none is available, there will be memory 
 * corruption. Probably.
 * 
 * @return UINT8 
 */
UINT8 borrow_oam_id();

/**
 * @brief Return an OAM id
 * 
 * @param id 
 */
void free_oam_id(UINT8 id);

#endif