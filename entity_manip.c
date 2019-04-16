#include <stdlib.h>
#include <stdbool.h>
#include "game.h"
#include "game_constants.h"
#include "entity_manip.h"

/*** Methods to change entities' slab ***
* They must be given a "self" of type "entity". eg : move_up(pacman->self) 
* If one tries to move where one cannot move, nothing happens.
*/

struct entity* entity_initiate(struct slab* current_slab){
    struct entity* e = malloc(sizeof(struct entity));
    e->speed = 0;
    e->current_slab = current_slab;
    e->dir = 0;
    return e;
}

struct pacman* pacman_initiate(struct slab* current_slab){
    struct pacman* p = malloc(sizeof(struct pacman));
    p->self = entity_initiate(current_slab);
    return p;
}

struct ghost* ghost_initiate(struct slab* current_slab){
    struct ghost* g = malloc(sizeof(struct ghost));
    g->self = entity_initiate(current_slab);
    g->target = NULL;
    g->state = NORMAL;
    return g;
}

void move_up(struct entity* e){
    struct slab* next = e->current_slab->up;
    if (next->type != WALL) {e->current_slab = next;}
}

void move_down(struct entity* e){
    struct slab* next = e->current_slab->down;
    if (next->type != WALL) {e->current_slab = next;}
}

void move_right(struct entity* e){
    struct slab* next = e->current_slab->right;
    if (next->type != WALL) {e->current_slab = next;}
}

void move_left(struct entity* e){
    struct slab* next = e->current_slab->left;
    if (next->type != WALL) {e->current_slab = next;}
}