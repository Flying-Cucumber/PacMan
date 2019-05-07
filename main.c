#include <stdbool.h>
#include <stdio.h>
#include <SDL/SDL.h>
#include "terrain_manipulation.h"
#include "entity_manip.h"
#include "game.h"
#include "chase.h"
#include "game_constants.h"
#include "graphics.h"


struct game* initiate_game(){
    struct game* g = malloc(sizeof(struct game));

    g->t = initiate_terrain();

    g->p = pacman_initiate(g->t->spawn_slab);

    g->pinky = ghost_initiate(g->t->ghost_house);
    g->blinky = ghost_initiate(g->t->ghost_house);
    g->inky = ghost_initiate(g->t->ghost_house);
    g->clyde = ghost_initiate(g->t->ghost_house);
    
    g->f = NULL;
    
    g->score = 0;
    g->lives = 3;
    
    return g;
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

bool is_colliding(struct entity* entity_1, struct entity* entity_2){
    return (bool) (entity_1->current_slab == entity_2->current_slab);
}

void set_ghost_state(struct game* g, int state){
    g->pinky->state = state;
    g->blinky->state = state;
    g->inky->state = state;
    g->clyde->state = state;
}

void pacman_move(struct game* g){
    
    // Gestion du déplacement physique
    switch (g->p->self->dir)
    {
        case UP:
            if (g->p->self->current_slab->up->type != GHOST_HOUSE){
                move_up(g->p->self);
            }
            break;
        case DOWN:
            if (g->p->self->current_slab->down->type != GHOST_HOUSE){
                move_down(g->p->self);
            }
            break;
        case RIGHT:
            if (g->p->self->current_slab->right->type != GHOST_HOUSE){
                move_right(g->p->self);
            }
            break;
        case LEFT:
            if (g->p->self->current_slab->left->type != GHOST_HOUSE){
                move_left(g->p->self);
            }
            break;
        default:
            break;
    }
    
    // Gestion des interactions avec la nouvelle case
    switch (g->p->self->current_slab->type)
    {
        case PAC_GUM:
            g->p->self->current_slab->type = PATH;
            g->score += 30;
            break;
        case SUPER_PAC_GUM:
            g->p->self->current_slab->type = PATH;
            set_ghost_state(g, SCARED);
            g->score += 100;
        default:
            break;
    }

    // Gestion des interactions avec les entités
    // Avec Pinky
    if (is_colliding(g->p->self, g->pinky->self)){
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
    if (is_colliding(g->p->self, g->blinky->self)){
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
    if (is_colliding(g->p->self, g->inky->self)){
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
    if (is_colliding(g->p->self, g->clyde->self)){
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
    if (g->f != NULL && is_colliding(g->p->self, g->f->self)){
        g->score += g->f->points;
        g->f = NULL;
    }
}

int main(){
    struct game* g = initiate_game();
    if (g == NULL){
        printf("Error: Game structure couldn't be initialized!");
        return (-1);
    }
    printf("Chargement terminé, Pacman en (%d, %d), Pinky en (%d, %d), Blinky en (%d, %d), Inky en (%d, %d) et Clyde en (%d, %d)\n", g->p->self->current_slab->x, g->p->self->current_slab->y, g->pinky->self->current_slab->x, g->pinky->self->current_slab->y, g->blinky->self->current_slab->x, g->blinky->self->current_slab->y, g->inky->self->current_slab->x, g->inky->self->current_slab->y, g->clyde->self->current_slab->x, g->clyde->self->current_slab->y);
    /*
    chase_mode(g);
    printf("Après un tour, Pacman en (%d, %d), Pinky en (%d, %d), Blinky en (%d, %d), Inky en (%d, %d) et Clyde en (%d, %d)\n", g->p->self->current_slab->x, g->p->self->current_slab->y, g->pinky->self->current_slab->x, g->pinky->self->current_slab->y, g->blinky->self->current_slab->x, g->blinky->self->current_slab->y, g->inky->self->current_slab->x, g->inky->self->current_slab->y, g->clyde->self->current_slab->x, g->clyde->self->current_slab->y);
    chase_mode(g);
    printf("Après deux tours, Pacman en (%d, %d), Pinky en (%d, %d), Blinky en (%d, %d), Inky en (%d, %d) et Clyde en (%d, %d)\n", g->p->self->current_slab->x, g->p->self->current_slab->y, g->pinky->self->current_slab->x, g->pinky->self->current_slab->y, g->blinky->self->current_slab->x, g->blinky->self->current_slab->y, g->inky->self->current_slab->x, g->inky->self->current_slab->y, g->clyde->self->current_slab->x, g->clyde->self->current_slab->y);
    */

    SDL_Event event;
    int game_on = 1;

    if (SDL_Init(SDL_INIT_VIDEO | SDL_DOUBLEBUF) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Intelligence artificielle pour Hanabi", NULL);

    SDL_Surface *ecran = SDL_SetVideoMode((g->t->size_x * SLAB_SIZE), (g->t->size_y * SLAB_SIZE), 32, SDL_HWSURFACE);

    if (ecran == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    start_interface(ecran, g);

    int temps_actuel = 0, temps_precedent = 0;

    while (game_on){
        SDL_PollEvent(&event);
        
        if (temps_actuel - temps_precedent > 30){
            switch (event.type)
            {
                case SDL_QUIT:
                    game_on = 0;
                    break;
                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_UP:
                            move_up(g->p->self);
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
            temps_actuel = SDL_GetTicks();    
        }else{
            SDL_Delay(30 - (temps_actuel - temps_precedent));
        }
        SDL_Delay(20);
    }



    SDL_Quit();

    return 0;
}