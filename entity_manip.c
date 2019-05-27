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

//////////////////////////////////////////////////////////////////
//////////////// Methods to initiate the entities ////////////////
//////////////////////////////////////////////////////////////////

Entity* entity_initiate(Slab* current_slab){
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
    if (g == NULL){
        printf("Error: couldn't initialize a ghost!");
        exit(1);
    }
    g->self = entity_initiate(current_slab);
    g->state = NORMAL;
    return g;
}
//////////////////////////////////////////////////////////////////
////////////////// Methods to move the entities //////////////////
////// The move_"dir" functions below do not check anything //////

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

Slab* move(Entity* e){
    /* Moves an entity according to its current direction.
    * !!! Does not check if the move is legit !!! 
    * This is done in chase.c 
    * Returns the (newly) previous slab of e */
    Slab* previous_slab = e->current_slab;
    switch (e->dir){
        case UP:
            move_up(e);
            break;
        case RIGHT:
            move_right(e);
            break;
        case DOWN:
            move_down(e);
            break;
        case LEFT:
            move_left(e);
            break;
        default:
            break;
    }
    return previous_slab;
}

//////////////////////////////////////////////////////////////////
////////////////////// Higher game methods ///////////////////////
//////////////////////////////////////////////////////////////////

void set_ghost_state(struct game* g, int state){
    g->pinky->state = state;
    g->blinky->state = state;
    g->inky->state = state;
    g->clyde->state = state;
}

void move_all_ghosts(struct game* g, Slab** slabs_to_repaint){
    slabs_to_repaint[1] = move(g->blinky->self);
    slabs_to_repaint[2] = move(g->pinky->self);
    slabs_to_repaint[3] = move(g->inky->self);
    slabs_to_repaint[4] = move(g->clyde->self);
}

bool is_colliding(struct entity* entity_1, struct entity* entity_2){
    return (bool) (entity_1->current_slab == entity_2->current_slab);
}

void death(struct game* g){ // Permet de gérer le cas de collision avec un fantôme
    g->lives -= 1;
    if (g->lives >= 0){
        g->p->self->current_slab = g->t->spawn_slab;
        g->pinky->self->current_slab = g->t->ghost_house;
        g->blinky->self->current_slab = g->t->ghost_house;
        g->inky->self->current_slab = g->t->ghost_house;
        g->clyde->self->current_slab = g->t->ghost_house;
    }else{
        printf("Game Over");
    }
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

void pacman_move(Pacman* p, int direction, Slab** slabs_to_repaint){
    /* Changes both Pac-Man's dir and current_slab attributes.
    * Tries to move Pac-Man in its current (new) direction;
     * previous direction and tries again to move once;
    * if that move is still illegal, does nothing.
    * direction is the direction wanted by user via keyboard input */
    struct slab* current_slab = p->self->current_slab;
    struct slab* next_slab;

    switch (direction){
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
    if (Is_Path(next_slab)){
        p->self->dir = direction;
        p->self->current_slab = next_slab;
    }

    // Else, try to move in previous direction
    else{
        next_slab = move_straight(current_slab, p->self->dir, 1);
        if (Is_Path(next_slab)){
            p->self->current_slab = next_slab;
        }
    }
    // ELselse, do nothing
    slabs_to_repaint[0] =  current_slab;
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
    
    SDL_FreeSurface(current_slab->objet);

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

void animations_update(Entity* e){
    /* Updates an entity'sanimations positions */

    int e_x, e_y;
    e_x = e->current_slab->x;
    e_y = e->current_slab->y;

    // Change all animations' positions... ...
    e->anim_up->position.x = e_x * SLAB_SIZE;
    e->anim_up->position.y = e_y * SLAB_SIZE;

    e->anim_right->position.x = e_x * SLAB_SIZE;
    e->anim_right->position.y = e_y * SLAB_SIZE;

    e->anim_down->position.x = e_x * SLAB_SIZE;
    e->anim_down->position.y = e_y * SLAB_SIZE;

    e->anim_left->position.x = e_x * SLAB_SIZE;
    e->anim_left->position.y = e_y * SLAB_SIZE;
}

void update_all_animations(Game* g){
    animations_update(g->p->self);
    animations_update(g->blinky->self);
    animations_update(g->pinky->self);
    animations_update(g->clyde->self);
}