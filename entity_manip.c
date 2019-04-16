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
    g->state = NORMAL;
    return g;
}

// the move_"dir" functions below do not check anything
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

void move_ghost(struct ghost* ghost){
    switch (ghost->self->dir){
        case (UP):
            move_up(ghost->self);
        case (RIGHT):
            move_right(ghost->self);
        case (DOWN):
            move_down(ghost->self);
        default:
            move_left(ghost->self);
    }
}

void move_all_ghosts(struct game* g){
    move_ghost(g->blinky);
    move_ghost(g->pinky);
    move_ghost(g->inky);
}