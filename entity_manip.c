#include <stdlib.h>
#include <stdbool.h>
#include <SDL/SDL.h>
#include "game.h"
#include "terrain_manipulation.h"
#include "game_constants.h"
#include "entity_manip.h"

/*** Methods to change entities' slab ***
* If one tries to move where one cannot move, an unhandled error appears!
*/

//////////////// Methods to initiate the entities ////////////////
Entity* entity_initiate(struct slab* current_slab){
    Entity* e = malloc(sizeof(Entity));
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
/* the move_"dir" functions below do not check anything! */
void move_up(Entity* e){
    e->current_slab = e->current_slab->up;
}

void move_down(Entity* e){
    e->current_slab = e->current_slab->down;
}

void move_right(Entity* e){
    e->current_slab = e->current_slab->right;
}

void move_left(Entity* e){
    e->current_slab = e->current_slab->left;
}

void move(Entity* e){
    /* Moves an entity according to its current direction.
    * Does not check if the move is legit */
    switch (e->dir){
        case UP:
            move_up(e);
        case RIGHT:
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

void set_dir(SDLKey pressed, Entity* e){
    /* Changes e's "dir" attribute according to keyboard input "pressed" */
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

void pacman_move_startegy(Entity* e, int new_dir){
    /* Changes both Pac-Man's dir and current_slab attributes.
    * Tries to move Pac-Man in its current (new) direction;
    * if the incident move is illegal, reverts to 
    * previous direction and tries again to move once;
    * if that move is still illegal, does nothing.
    * new_dir is the direction wanted by user via keyboard input */
    struct slab* current_slab = e->current_slab;
    struct slab* next_slab;

    switch (new_dir){
        case UP: 
            next_slab = current_slab->up;
            break;
        case RIGHT: 
            next_slab = current_slab->right;
            break;
        case DOWN: 
            next_slab = current_slab->down;
            break;
        case LEFT: 
            next_slab = current_slab->left;
            break;
        default:
            break;
    }

    // If next slab in wanted direction is a path, move is legit
    if (slab_Is_Path(next_slab)){
        e->dir = new_dir;
        move(e);
    }

    // Else, try to move in previous direction
    else{
        next_slab = fieldBrowsing(current_slab, e->dir, 1);
        if (slab_Is_Path(next_slab)){
            move(e);
        }
    }
    // ELselse, do nothing
}

void pacman_move(Game* g, SDL_Event event){
    /* This method is called in case of SDL_KEYDOWN event. 
    * It moves Pac-Man according to user input. */
    Entity* p_self = g->p->self;
    switch (event.key.keysym.sym){
        case SDLK_UP:
            pacman_move_startegy(p_self, UP);
            break;
        case SDLK_RIGHT:
            pacman_move_startegy(p_self, RIGHT);
            break;
        case SDLK_DOWN:
            pacman_move_startegy(p_self, DOWN);
            break;
        default:
            pacman_move_startegy(p_self, LEFT);
            break;
    }
}

void pacman_interaction(Game* g){
    Entity* self = g->p->self;
    struct slab* current_slab = self->current_slab;
    
    
    // Gestion des interactions avec la nouvelle case
    switch (current_slab->type)
    {
        case PAC_GUM:
            current_slab->type = PATH;
            g->score += 30;
            break;
        case SUPER_PAC_GUM:
            current_slab->type = PATH;
            set_ghost_state(g, SCARED);
            g->score += 100;
        default:
            break;
    }

    // Gestion des interactions avec les entités
    // Avec Pinky
    if (is_colliding(self, g->pinky->self)){
        switch (g->pinky->state)
        {
            case NORMAL:
                death(g);
                break;
            case SCARED:
                g->pinky->state = DEAD;
                g->score += 100;
                break;
            default:
                break;
        }
    }

    // Avec Blinky
    if (is_colliding(self, g->blinky->self)){
        switch (g->blinky->state)
        {
            case NORMAL:
                death(g);
                break;
            case SCARED:
                g->blinky->state = DEAD;
                g->score += 100;
                break;
            default:
                break;
        }
    }

    // Avec Inky
    if (is_colliding(self, g->inky->self)){
        switch (g->inky->state)
        {
            case NORMAL:
                death(g);
                break;
            case SCARED:
                g->inky->state = DEAD;
                g->score += 100;
                break;
            default:
                break;
        }
    }

    // Avec Clyde
    if (is_colliding(self, g->clyde->self)){
        switch (g->clyde->state)
        {
            case NORMAL:
                death(g);
                break;
            case SCARED:
                g->clyde->state = DEAD;
                g->score += 100;
                break;
            default:
                break;
        }
    }

    // Avec les fruits
    if (g->f != NULL && is_colliding(self, g->f->self)){
        g->score += g->f->points;
        g->f = NULL;
    }
}