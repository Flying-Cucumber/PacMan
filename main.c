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

int main(){

///////////////////////////////////////////////////////
//////////   Initialisation des variable    ///////////
///////////////////////////////////////////////////////

    int direction = UP;

    Slab** slabs_to_repaint[10];
    for(int i = 0; i < 10; i++){
        slabs_to_repaint[i] = NULL;
    }

///////////////////////////////////////////////////////
/////////   Initialisation des structures    //////////
///////////////////////////////////////////////////////

    struct game* g = initiate_game();
    if (g == NULL){
        printf("Error: Game structure couldn't be initialized!");
        exit (-1);
    }
    printf("Chargement terminé, Pacman en (%d, %d), Pinky en (%d, %d), Blinky en (%d, %d), Inky en (%d, %d) et Clyde en (%d, %d)\n", g->p->self->current_slab->x, g->p->self->current_slab->y, g->pinky->self->current_slab->x, g->pinky->self->current_slab->y, g->blinky->self->current_slab->x, g->blinky->self->current_slab->y, g->inky->self->current_slab->x, g->inky->self->current_slab->y, g->clyde->self->current_slab->x, g->clyde->self->current_slab->y);

    slabs_to_repaint[0] = g->blinky->self->current_slab;

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
        
        if (temps_actuel - temps_precedent > REFRESH_RATE){
            
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

            //draw_slab(ecran, slabs_to_repaint[0]);
            slabs_to_repaint[0] = chase_mode(g);
            Slab* p_previous_slab = pacman_move(g->p, direction);
            pacman_interaction(g);
            paint_entities(ecran, g, p_previous_slab);
            SDL_Flip(ecran);

        }else{
            
            SDL_Delay(REFRESH_RATE - (temps_actuel - temps_precedent));

        }
        temps_actuel = SDL_GetTicks();
    }

    SDL_Quit();

    return 0;
}