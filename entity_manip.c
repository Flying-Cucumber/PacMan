#include <stdlib.h>
#include "terrain.h"
#include "entities.h"
#include "entity_manip.h"

/*** Methods to change entities' slab ***
* They must be given a "self" of type "entity". eg : move_up(pacman->self) 
* If one tries to move where one cannot move, nothing happens.
*/

void move_up(struct entity* e){
    struct slab* next = e->current_slab->up;
    if (next->type == PATH) {e->current_slab = next;}
}

void move_down(struct entity* e){
    struct slab* next = e->current_slab->down;
    if (next->type == PATH) {e->current_slab = next;}
}

void move_right(struct entity* e){
    struct slab* next = e->current_slab->right;
    if (next->type == PATH) {e->current_slab = next;}
}

void move_left(struct entity* e){
    struct slab* next = e->current_slab->left;
    if (next->type == PATH) {e->current_slab = next;}
}