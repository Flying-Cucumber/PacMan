#include <stdlib.h>
#include "terrain.h"
#include "entities.h"
#include "entity_manip.h"

/* Methods to move entities.
They must be given a "self" of type "entity". eg : move_up(pacman->self) 
If one tries to move where one cannot move, nothing happens.
*/

void move_up(struct entity* e){
    struct slab* target = e->current_slab->up;
    if (target->_type == PATH) {e->current_slab = target;}
}

void move_down(struct entity* e){
    struct slab* target = e->current_slab->down;
    if (target->_type == PATH) {e->current_slab = target;}
}

void move_right(struct entity* e){
    struct slab* target = e->current_slab->right;
    if (target->_type == PATH) {e->current_slab = target;}
}

void move_left(struct entity* e){
    struct slab* target = e->current_slab->left;
    if (target->_type == PATH) {e->current_slab = target;}
}