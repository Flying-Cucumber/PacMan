#include <stdlib.h>
#include "terrain.h"
#include "entities.h"
#include "entity_manip.h"

/* Methods to move entities.
They must be given a "self" of type "entity".
eg : move_up(pacman->self) */

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