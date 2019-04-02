#ifndef __ENTITY_MANIP_H__
#define __ENTITY_MANIP_H__

#include <stdlib.h>
#include "terrain.h"
#include "entities.h"

void move_up(struct entity* e){
    e->current_slab = e->current_slab->up;
}

void move_down(struct entity* e){
    e->current_slab = e->current_slab->down;
}

void move_right(struct entity* e){
    e->current_slab = e->current_slab->right;
}

void move_left(struct entity* e){
    e->current_slab = e->current_slab->left;
}

#endif
