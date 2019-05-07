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

int main(){

///////////////////////////////////////////////////////
//////////   Initialisation des variable    ///////////
///////////////////////////////////////////////////////

    int direction;
    Slab* slabs_to_repaint[10];

///////////////////////////////////////////////////////
/////////   Initialisation des structures    //////////
///////////////////////////////////////////////////////

    struct game* g = initiate_game();
    if (g == NULL){
        printf("Error: Game structure couldn't be initialized!");
        exit (-1);
    }
    printf("Chargement terminé, Pacman en (%d, %d), Pinky en (%d, %d), Blinky en (%d, %d), Inky en (%d, %d) et Clyde en (%d, %d)\n", g->p->self->current_slab->x, g->p->self->current_slab->y, g->pinky->self->current_slab->x, g->pinky->self->current_slab->y, g->blinky->self->current_slab->x, g->blinky->self->current_slab->y, g->inky->self->current_slab->x, g->inky->self->current_slab->y, g->clyde->self->current_slab->x, g->clyde->self->current_slab->y);

///////////////////////////////////////////////////////
////////////   Initialisation graphique    ////////////
///////////////////////////////////////////////////////

    SDL_Event event;
    int game_on = 1;

    if (SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_WM_SetCaption("Intelligence artificielle pour Hanabi", NULL);

    SDL_Surface *ecran = SDL_SetVideoMode((g->t->size_x * SLAB_SIZE), (g->t->size_y * SLAB_SIZE), 32, SDL_HWSURFACE | SDL_DOUBLEBUF);

    if (ecran == NULL){
        fprintf(stderr, "Impossible de charger le mode vidéo : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    start_interface(ecran, g);

    int temps_actuel = 0, temps_precedent = 0;

///////////////////////////////////////////////////////
//////////////   Boucle des événements    /////////////
///////////////////////////////////////////////////////

    while (game_on){

        SDL_PollEvent(&event);
        
        if (temps_actuel - temps_precedent > 30){
            
            switch (event.type){
                case SDL_QUIT:
                    game_on = 0;
                    break;

                case SDL_KEYDOWN:
                    switch (event.key.keysym.sym){
                        case SDLK_UP:
                            direction = UP;
                            break;
                        case SDLK_RIGHT:
                            direction = RIGHT;
                            break;
                        case SDLK_DOWN:
                            direction = DOWN;
                            break;
                        case SDLK_LEFT:
                            direction = LEFT;
                            break;
                        default:
                            break;
                        }
                    break;
                default:
                    break;
            }    

            temps_precedent = temps_actuel;

        }else{
            
            SDL_Delay(30 - (temps_actuel - temps_precedent));

        }
        temps_actuel = SDL_GetTicks();

        pacman_move(g->p, direction);
        pacman_interaction(g);
        paint_entities(ecran, g);
        SDL_Flip(ecran);
    }

    SDL_Quit();

    return 0;
}