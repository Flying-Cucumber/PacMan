#include <SDL/SDL.h>
#include "game.h"

void start_interface(struct game* g){
    
    if (SDL_Init(SDL_INIT_VIDEO) == -1){
        fprintf(stderr, "Erreur d'initialisation de la SDL: %s\n", SDL_GetError);
        exit(EXIT_FAILURE);
    }

    SDL_Quit();
}