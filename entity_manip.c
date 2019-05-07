#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include "game.h"
#include "game_constants.h"
#include "entity_manip.h"

/*** Methods to change entities' slab ***
* If one tries to move where one cannot move, an unhandled error appears!
*/

//////////////// Methods to initiate the entities ////////////////
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

//////////////// Methods to move the entities ////////////////
// the move_"dir" functions below do not check anything!
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

void move(struct entity* e){
    /* Moves an entity according to its current direction */
    switch (e->dir){
        case (UP):
            move_up(e);
        case (RIGHT):
            move_right(e);
        case (DOWN):
            move_down(e);
        default:
            move_left(e);
    }
}

//////////////// Higher game methods ////////////////

void move_all_ghosts(struct game* g){
    move(g->blinky->self);
    move(g->pinky->self);
    move(g->inky->self);
}

void set_dir(SDLKey pressed, struct entity* e){
    /* Changes e's "dir" attribute according to "pressed" */
    switch (pressed){
        case SDLK_UP:
            e->dir = UP;
        case SDLK_RIGHT:
            e->dir = RIGHT;
        case SDLK_DOWN:
            e->dir = DOWN;
        default:
            e->dir = LEFT;
    }
}